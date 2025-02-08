# Stream Log Exporter for OpenTelemetry

## Why?

Because it is often faster and easier to look at the program's stderr to see what is going on.

## Usage

```cmake
find_package(psb-opentelemetry-logs-exporter-stream CONFIG REQUIRED)
target_link_libraries("${PROJECT_NAME}" PRIVATE psb::opentelemetry::logs-exporter-stream)
```

```cpp
#include <opentelemetry/logs/provider.h>
#include <opentelemetry/logs/severity.h>
#include <opentelemetry/sdk/logs/logger_provider.h>
#include <opentelemetry/sdk/logs/simple_log_record_processor.h>
#include <psb/opentelemetry/log_record_stream_exporter_factory.h>

int main()
{
    auto exporter  = psb::opentelemetry::LogRecordStreamExporterFactory::Create();
    auto processor = std::make_unique<opentelemetry::sdk::logs::SimpleLogRecordProcessor>(std::move(exporter));
    auto provider  = std::make_shared<opentelemetry::sdk::logs::LoggerProvider>(std::move(processor));
    opentelemetry::logs::Provider::SetLoggerProvider(
        std::static_pointer_cast<opentelemetry::logs::LoggerProvider>(provider)
    );

    auto logger = opentelemetry::logs::Provider::GetLoggerProvider()->GetLogger("Logger Name", "Library Name", "1.0");

    logger->Debug("Hello World");

    return 0;
}
```
