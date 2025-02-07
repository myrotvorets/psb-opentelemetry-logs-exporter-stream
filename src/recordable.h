#ifndef C3F17F64_9C46_4BB6_8F0D_939E493F1173
#define C3F17F64_9C46_4BB6_8F0D_939E493F1173

#include <cstdint>
#include <string>
#include <unordered_map>

#include <opentelemetry/common/attribute_value.h>
#include <opentelemetry/common/timestamp.h>
#include <opentelemetry/logs/severity.h>
#include <opentelemetry/nostd/string_view.h>
#include <opentelemetry/sdk/common/attribute_utils.h>
#include <opentelemetry/sdk/instrumentationscope/instrumentation_scope.h>
#include <opentelemetry/sdk/logs/recordable.h>
#include <opentelemetry/sdk/resource/resource.h>
#include <opentelemetry/trace/span_id.h>
#include <opentelemetry/trace/trace_flags.h>
#include <opentelemetry/trace/trace_id.h>

namespace psb::opentelemetry {

class Recordable final : public ::opentelemetry::sdk::logs::Recordable {
public:
    void SetTimestamp(::opentelemetry::common::SystemTimestamp timestamp) noexcept override;
    void SetObservedTimestamp(::opentelemetry::common::SystemTimestamp timestamp) noexcept override;
    void SetSeverity(::opentelemetry::logs::Severity severity) noexcept override;
    void SetBody(const ::opentelemetry::common::AttributeValue& message) noexcept override;
    void SetAttribute(
        ::opentelemetry::nostd::string_view name, const ::opentelemetry::common::AttributeValue& value
    ) noexcept override;
    void SetEventId(std::int64_t id, ::opentelemetry::nostd::string_view name = {}) noexcept override;
    void SetTraceId(const ::opentelemetry::trace::TraceId& trace_id) noexcept override;
    void SetSpanId(const ::opentelemetry::trace::SpanId& span_id) noexcept override;
    void SetTraceFlags(const ::opentelemetry::trace::TraceFlags& trace_flags) noexcept override;
    void SetResource(const ::opentelemetry::sdk::resource::Resource& resource) noexcept override;
    void SetInstrumentationScope(
        const ::opentelemetry::sdk::instrumentationscope::InstrumentationScope& instrumentation_scope
    ) noexcept override;

    [[nodiscard]] ::opentelemetry::common::SystemTimestamp GetTimestamp() const noexcept;
    [[nodiscard]] ::opentelemetry::logs::Severity GetSeverity() const noexcept;
    [[nodiscard]] const std::string& GetBody() const noexcept;
    [[nodiscard]] const std::unordered_map<std::string, std::string>& GetAttributes() const noexcept;
    [[nodiscard]] const ::opentelemetry::sdk::instrumentationscope::InstrumentationScope*
    GetInstrumentationScope() const noexcept;

private:
    const ::opentelemetry::sdk::instrumentationscope::InstrumentationScope* m_instrumentation_scope = nullptr;
    ::opentelemetry::common::SystemTimestamp m_timestamp;
    std::string m_body;
    std::unordered_map<std::string, std::string> m_attributes;
    ::opentelemetry::logs::Severity m_severity = ::opentelemetry::logs::Severity::kInvalid;
};

}  // namespace psb::opentelemetry

#endif /* C3F17F64_9C46_4BB6_8F0D_939E493F1173 */
