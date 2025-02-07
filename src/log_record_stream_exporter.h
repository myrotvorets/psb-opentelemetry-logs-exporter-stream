#ifndef BB1766CD_2D39_4A14_83BC_75E2A0AE3585
#define BB1766CD_2D39_4A14_83BC_75E2A0AE3585

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <ostream>

#include <opentelemetry/nostd/string_view.h>
#include <opentelemetry/sdk/logs/exporter.h>

namespace psb::opentelemetry {

class Recordable;
class LogRecordStreamExporter final : public ::opentelemetry::sdk::logs::LogRecordExporter {
    using recordable_t  = std::unique_ptr<::opentelemetry::sdk::logs::Recordable>;
    using recordables_t = ::opentelemetry::nostd::span<recordable_t>;

public:
    explicit LogRecordStreamExporter(std::ostream& stream = std::cerr) noexcept;

    std::unique_ptr<::opentelemetry::sdk::logs::Recordable> MakeRecordable() noexcept override;

    ::opentelemetry::sdk::common::ExportResult Export(const recordables_t& records) noexcept override;

    bool ForceFlush(std::chrono::microseconds timeout = std::chrono::microseconds::max()) noexcept override;
    bool Shutdown(std::chrono::microseconds timeout = std::chrono::microseconds::max()) noexcept override;

private:
    std::ostream* m_stream;
    std::atomic_bool m_is_shutdown = false;

    void process_record(const Recordable* record);
};

}  // namespace psb::opentelemetry

#endif /* BB1766CD_2D39_4A14_83BC_75E2A0AE3585 */
