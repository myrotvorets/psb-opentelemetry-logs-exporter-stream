#ifndef CCEB82A8_1825_4415_9F2D_9AFE8D9A76DE
#define CCEB82A8_1825_4415_9F2D_9AFE8D9A76DE

#include <memory>
#include <opentelemetry/sdk/logs/exporter.h>
#include "export.h"

namespace psb::opentelemetry {

class PSB_OPENTELEMETRY_EXPORTER_STREAM_LOGS_EXPORT LogRecordStreamExporterFactory {
public:
    static std::unique_ptr<::opentelemetry::sdk::logs::LogRecordExporter> Create();
    static std::unique_ptr<::opentelemetry::sdk::logs::LogRecordExporter> Create(std::ostream& stream);
};

}  // namespace psb::opentelemetry

#endif /* CCEB82A8_1825_4415_9F2D_9AFE8D9A76DE */
