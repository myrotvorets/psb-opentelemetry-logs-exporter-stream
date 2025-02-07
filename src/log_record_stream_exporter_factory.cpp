#include "log_record_stream_exporter_factory.h"

#include <iostream>

#include "log_record_stream_exporter.h"

namespace psb::opentelemetry {

// GCOVR_EXCL_START
std::unique_ptr<::opentelemetry::sdk::logs::LogRecordExporter> LogRecordStreamExporterFactory::Create()
{
    return LogRecordStreamExporterFactory::Create(std::cerr);
}
// GCOVR_EXCL_STOP

std::unique_ptr<::opentelemetry::sdk::logs::LogRecordExporter>
LogRecordStreamExporterFactory::Create(std::ostream& stream)
{
    return std::make_unique<LogRecordStreamExporter>(stream);
}

}  // namespace psb::opentelemetry
