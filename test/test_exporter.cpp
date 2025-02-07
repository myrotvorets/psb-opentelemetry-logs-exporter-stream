#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>
#include <format>
#include <initializer_list>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <opentelemetry/logs/event_id.h>
#include <opentelemetry/logs/provider.h>
#include <opentelemetry/logs/severity.h>
#include <opentelemetry/sdk/logs/logger_provider.h>
#include <opentelemetry/sdk/logs/simple_log_record_processor.h>
#include <opentelemetry/trace/span_context.h>
#include <opentelemetry/trace/span_id.h>
#include <opentelemetry/trace/trace_id.h>

#include "log_record_stream_exporter.h"
#include "log_record_stream_exporter_factory.h"

class StreamExporterTest : public testing::Test {
public:
    std::string actual() const { return this->m_stream.str(); }
    psb::opentelemetry::LogRecordStreamExporter* exporter() const { return this->m_exporter; }

protected:
    void SetUp() override
    {
        auto exporter    = psb::opentelemetry::LogRecordStreamExporterFactory::Create(this->m_stream);
        this->m_exporter = dynamic_cast<psb::opentelemetry::LogRecordStreamExporter*>(exporter.get());

        auto processor = std::make_unique<opentelemetry::sdk::logs::SimpleLogRecordProcessor>(std::move(exporter));
        auto provider  = std::make_shared<opentelemetry::sdk::logs::LoggerProvider>(std::move(processor));
        opentelemetry::logs::Provider::SetLoggerProvider(
            std::static_pointer_cast<opentelemetry::logs::LoggerProvider>(provider)
        );
    }

    void TearDown() override
    {
        const std::shared_ptr<opentelemetry::logs::LoggerProvider> provider;
        opentelemetry::logs::Provider::SetLoggerProvider(provider);
        this->m_stream.str("");
    }

private:
    std::stringstream m_stream;
    psb::opentelemetry::LogRecordStreamExporter* m_exporter = nullptr;
};

constexpr auto observed_timestamp             = std::chrono::nanoseconds(1'645'666'800'000'000'000ULL);
constexpr std::string_view expected_timestamp = "2022-02-24 01:40:00";

TEST_F(StreamExporterTest, Basic)
{
    const std::string expected_severity    = "DEBUG";
    const std::string expected_logger_name = "Logger";
    const std::string expected_message     = "debug message";

    auto logger = opentelemetry::logs::Provider::GetLoggerProvider()->GetLogger(expected_logger_name);
    logger->Debug(expected_message, opentelemetry::common::SystemTimestamp(observed_timestamp));

    const std::string expected =
        std::format("{} {} [{}]: {}\n", expected_timestamp, expected_severity, expected_logger_name, expected_message);
    EXPECT_EQ(this->actual(), expected);
}

TEST_F(StreamExporterTest, UnknownSeverity)
{
    const auto expected_severity           = std::string{opentelemetry::logs::SeverityNumToText[0]};
    const std::string expected_logger_name = "Logger";
    const std::string expected_message     = "debug message";

    auto logger = opentelemetry::logs::Provider::GetLoggerProvider()->GetLogger(expected_logger_name);
    logger->EmitLogRecord(
        expected_message, opentelemetry::common::SystemTimestamp(observed_timestamp),
        static_cast<opentelemetry::logs::Severity>(-1)
    );

    const std::string expected =
        std::format("{} {} [{}]: {}\n", expected_timestamp, expected_severity, expected_logger_name, expected_message);
    EXPECT_EQ(this->actual(), expected);
}

TEST_F(StreamExporterTest, Instrumentation)
{
    const std::string expected_severity       = "DEBUG";
    const std::string expected_logger_name    = "Logger";
    const std::string expected_logger_version = "19.83";
    const std::string expected_message        = "debug message";

    auto logger = opentelemetry::logs::Provider::GetLoggerProvider()->GetLogger(
        expected_logger_name, expected_logger_name, expected_logger_version
    );
    logger->Debug(expected_message, opentelemetry::common::SystemTimestamp(observed_timestamp));

    const std::string expected = std::format(
        "{} {} [{}/{}]: {}\n", expected_timestamp, expected_severity, expected_logger_name, expected_logger_version,
        expected_message
    );

    EXPECT_EQ(this->actual(), expected);
}

TEST_F(StreamExporterTest, Shutdown)
{
    auto* exporter = this->exporter();
    exporter->Shutdown();

    const std::string expected_logger_name = "Logger";
    const std::string expected_message     = "debug message";

    auto logger = opentelemetry::logs::Provider::GetLoggerProvider()->GetLogger(expected_logger_name);
    logger->Debug(expected_message, opentelemetry::common::SystemTimestamp(observed_timestamp));

    const std::string expected;

    EXPECT_EQ(this->actual(), expected);
}

TEST_F(StreamExporterTest, Attributes)
{
    const std::string expected_logger_name;
    const std::string expected_message = "debug message";

    const std::vector<int> vec{1, 2, 3};
    const std::vector<int> empty{};
    const std::initializer_list<std::pair<std::string, opentelemetry::common::AttributeValue>> attrs{
        {"string", "string"}, {"boolean", true}, {"integer", 0}, {"double", 1.0}, {"vec", vec}, {"empty_vec", empty}
    };

    auto logger = opentelemetry::logs::Provider::GetLoggerProvider()->GetLogger(expected_logger_name);
    logger->Debug(expected_message, opentelemetry::common::SystemTimestamp(observed_timestamp), attrs);

    const std::string expected = "2022-02-24 01:40:00 DEBUG []: debug message\n"
                                 "    empty_vec: \n"
                                 "    vec: 1, 2, 3\n"
                                 "    double: 1.000000\n"
                                 "    integer: 0\n"
                                 "    boolean: true\n"
                                 "    string: string\n";

    EXPECT_EQ(this->actual(), expected);
}

TEST_F(StreamExporterTest, InvalidTrace)
{
    const std::string expected_severity    = "DEBUG";
    const std::string expected_logger_name = "Logger";
    const std::string expected_message     = "debug message";

    opentelemetry::trace::SpanContext ctx(true, false);

    auto logger = opentelemetry::logs::Provider::GetLoggerProvider()->GetLogger(expected_logger_name);
    logger->Debug(expected_message, opentelemetry::common::SystemTimestamp(observed_timestamp), ctx);

    const std::string expected =
        std::format("{} {} [{}]: {}\n", expected_timestamp, expected_severity, expected_logger_name, expected_message);
    EXPECT_EQ(this->actual(), expected);
}

TEST_F(StreamExporterTest, ValidTrace)
{
    const std::string expected_severity    = "DEBUG";
    const std::string expected_logger_name = "Logger";
    const std::string expected_message     = "debug message";

    std::array<const std::uint8_t, opentelemetry::trace::TraceId::kSize> tid_data{0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
                                                                                  0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
                                                                                  0x0C, 0x0D, 0x0E, 0x0F};
    std::array<const std::uint8_t, opentelemetry::trace::SpanId::kSize> sid_data{0x10, 0x11, 0x12, 0x13,
                                                                                 0x14, 0x15, 0x16, 0x17};

    opentelemetry::trace::TraceId tid(tid_data);
    opentelemetry::trace::SpanId sid(sid_data);

    std::array<char, 2L * opentelemetry::trace::TraceId::kSize> expected_tid{};
    std::array<char, 2L * opentelemetry::trace::SpanId::kSize> expected_sid{};
    tid.ToLowerBase16(expected_tid);
    sid.ToLowerBase16(expected_sid);

    auto logger = opentelemetry::logs::Provider::GetLoggerProvider()->GetLogger(expected_logger_name);
    logger->Debug(expected_message, opentelemetry::common::SystemTimestamp(observed_timestamp), sid, tid);

    const auto expected_sid_str =
        std::format("    span_id: {}\n", std::string(expected_sid.data(), expected_sid.size()));
    const auto expected_tid_str =
        std::format("    trace_id: {}\n", std::string(expected_tid.data(), expected_tid.size()));

    const std::string expected =
        std::format("{} {} [{}]: {}\n", expected_timestamp, expected_severity, expected_logger_name, expected_message);

    const auto actual = this->actual();
    EXPECT_TRUE(actual.starts_with(expected));
    // The order of span_id and trace_id is not guaranteed
    EXPECT_NE(actual.find(expected_sid_str), std::string::npos);
    EXPECT_NE(actual.find(expected_tid_str), std::string::npos);
}

TEST_F(StreamExporterTest, EventdId)
{
    const std::string expected_logger_name = "Logger";
    const std::string expected_message     = "debug message";

    opentelemetry::logs::EventId eid(24'081'990, "E-V-E-N-T");

    auto logger = opentelemetry::logs::Provider::GetLoggerProvider()->GetLogger(expected_logger_name);
    logger->Debug(eid, expected_message, opentelemetry::common::SystemTimestamp(observed_timestamp));

    const auto actual = this->actual();
    EXPECT_EQ(actual.find(std::to_string(eid.id_)), std::string::npos);
    EXPECT_EQ(actual.find(eid.name_.get()), std::string::npos);
}
