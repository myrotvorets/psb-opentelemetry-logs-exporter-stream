#include "recordable.h"

#include <array>
#include <cassert>
#include <numeric>

#include <opentelemetry/logs/severity.h>
#include <opentelemetry/nostd/span.h>
#include <opentelemetry/nostd/string_view.h>
#include <opentelemetry/nostd/variant.h>
#include <opentelemetry/trace/span_id.h>
#include <opentelemetry/trace/trace_id.h>

namespace {

struct stringifier {
    template<typename T>
    std::string operator()(const T& arg) const
    {
        return std::to_string(arg);
    }

    std::string operator()(bool arg) const { return arg ? "true" : "false"; }

    std::string operator()(const char* arg) const { return arg; }
    std::string operator()(const opentelemetry::nostd::string_view& arg) const { return {arg.data(), arg.size()}; }

    template<typename T>
    std::string operator()(const opentelemetry::nostd::span<T>& data) const
    {
        stringifier str;
        std::vector<std::string> strings;
        strings.reserve(data.size());
        for (const auto& item : data) {
            strings.push_back(str(item));
        }

        return !data.empty() ? std::accumulate(
                                   std::next(data.begin()), data.end(), str(*data.begin()),
                                   [&str](const std::string& a, const T& b) { return a + ", " + str(b); }
                               )
                             : std::string{};
    }
};

}  // namespace

namespace psb::opentelemetry {

using namespace ::opentelemetry;

void Recordable::SetTimestamp(opentelemetry::common::SystemTimestamp timestamp) noexcept
{
    this->m_timestamp = timestamp;
}

void Recordable::SetObservedTimestamp(opentelemetry::common::SystemTimestamp timestamp) noexcept
{
    this->m_timestamp = timestamp;
}

void Recordable::SetSeverity(opentelemetry::logs::Severity severity) noexcept
{
    this->m_severity = severity;
}

void Recordable::SetBody(const opentelemetry::common::AttributeValue& message) noexcept
{
    this->m_body = opentelemetry::nostd::visit(stringifier{}, message);
}

void Recordable::SetAttribute(
    opentelemetry::nostd::string_view name, const opentelemetry::common::AttributeValue& value
) noexcept
{
    const auto str = opentelemetry::nostd::visit(stringifier{}, value);

    this->m_attributes[std::string{name}] = str;
}

void Recordable::SetEventId(std::int64_t, opentelemetry::nostd::string_view) noexcept
{
    // We don't use event id
}

void Recordable::SetTraceId(const opentelemetry::trace::TraceId& trace_id) noexcept
{
    if (trace_id.IsValid()) {
        std::array<char, 2L * opentelemetry::trace::TraceId::kSize> buffer{};
        trace_id.ToLowerBase16(buffer);
        this->m_attributes["trace_id"] = std::string{buffer.data(), buffer.size()};
    }
}

void Recordable::SetSpanId(const opentelemetry::trace::SpanId& span_id) noexcept
{
    if (span_id.IsValid()) {
        std::array<char, 2L * opentelemetry::trace::SpanId::kSize> buffer{};
        span_id.ToLowerBase16(buffer);
        this->m_attributes["span_id"] = std::string{buffer.data(), buffer.size()};
    }
}

void Recordable::SetTraceFlags(const opentelemetry::trace::TraceFlags&) noexcept
{
    // We don't use trace flags
}

void Recordable::SetResource(const opentelemetry::sdk::resource::Resource&) noexcept
{
    // We don't use resource
}

void Recordable::SetInstrumentationScope(
    const opentelemetry::sdk::instrumentationscope::InstrumentationScope& instrumentation_scope
) noexcept
{
    this->m_instrumentation_scope = &instrumentation_scope;
}

opentelemetry::common::SystemTimestamp Recordable::GetTimestamp() const noexcept
{
    return this->m_timestamp;
}

opentelemetry::logs::Severity Recordable::GetSeverity() const noexcept
{
    return this->m_severity;
}

const std::string& Recordable::GetBody() const noexcept
{
    return this->m_body;
}

const std::unordered_map<std::string, std::string>& Recordable::GetAttributes() const noexcept
{
    return this->m_attributes;
}

const opentelemetry::sdk::instrumentationscope::InstrumentationScope*
Recordable::GetInstrumentationScope() const noexcept
{
    return this->m_instrumentation_scope;
}

}  // namespace psb::opentelemetry
