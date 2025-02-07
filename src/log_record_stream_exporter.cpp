#include "log_record_stream_exporter.h"

#include <ctime>
#include <ostream>
#include <string>

#include <opentelemetry/common/attribute_value.h>
#include <opentelemetry/sdk/instrumentationscope/instrumentation_scope.h>
#include <opentelemetry/sdk/logs/read_write_log_record.h>

#include "recordable.h"

namespace {

opentelemetry::nostd::string_view get_severity(opentelemetry::logs::Severity severity) noexcept
{
    auto num_severity = static_cast<std::size_t>(severity);
    if (num_severity >= std::extent_v<decltype(opentelemetry::logs::SeverityNumToText)>) {
        num_severity = 0;
    }

#pragma clang unsafe_buffer_usage begin
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index) -- index is checked
    return opentelemetry::logs::SeverityNumToText[num_severity];
#pragma clang unsafe_buffer_usage end
}

}  // namespace

namespace psb::opentelemetry {

using namespace ::opentelemetry;

LogRecordStreamExporter::LogRecordStreamExporter(std::ostream& stream) noexcept : m_stream(&stream) {}

std::unique_ptr<opentelemetry::sdk::logs::Recordable> LogRecordStreamExporter::MakeRecordable() noexcept
{
    return std::make_unique<Recordable>();
}

opentelemetry::sdk::common::ExportResult LogRecordStreamExporter::Export(
    const opentelemetry::nostd::span<std::unique_ptr<opentelemetry::sdk::logs::Recordable>>& records
) noexcept
{
    if (this->m_is_shutdown) {
        return opentelemetry::sdk::common::ExportResult::kFailure;
    }

    for (const auto& record : records) {
        if (const auto* log_record = dynamic_cast<const Recordable*>(record.get()); log_record != nullptr) {
            this->process_record(log_record);
        }
    }

    return opentelemetry::sdk::common::ExportResult::kSuccess;
}

bool LogRecordStreamExporter::ForceFlush(std::chrono::microseconds) noexcept
{
    return true;
}

bool LogRecordStreamExporter::Shutdown(std::chrono::microseconds) noexcept
{
    this->m_is_shutdown = true;
    return true;
}

void LogRecordStreamExporter::process_record(const Recordable* record)
{
    auto& stream = *this->m_stream;
    const auto time =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::time_point(record->GetTimestamp()));
    std::tm tm{};
    auto* const ptm = gmtime_r(&time, &tm);

    std::array<char, sizeof("YYYY-MM-DD HH:MM:SS")> date{};
    static_cast<void>(std::strftime(date.data(), date.size(), "%Y-%m-%d %H:%M:%S", ptm));

    stream << date.data() << " " << get_severity(record->GetSeverity());

    if (const auto* scope = record->GetInstrumentationScope(); scope != nullptr) {  // GCOVR_EXCL_BR_LINE
        stream << " [" << scope->GetName();
        if (const auto& version = scope->GetVersion(); !version.empty()) {
            stream << "/" << version;
        }

        stream << "]";
    }

    stream << ": " << record->GetBody() << "\n" << std::flush;
    if (const auto& attrs = record->GetAttributes(); !attrs.empty()) {
        for (const auto& [key, value] : attrs) {
            stream << "    " << key << ": " << value << "\n";
        }
    }

    stream << std::flush;
}

}  // namespace psb::opentelemetry
