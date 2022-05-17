#include "Logger.h"

Logger fileLogger;

Logger::Logger()
{
    logger = std::make_shared<spdlog::logger>("basic_logger", initFileSink());
    logger->set_level(spdlog::level::trace);
}

spdlog::logger* Logger::operator->()
{
    return logger.get();
}

std::shared_ptr<spdlog::sinks::basic_file_sink_mt> Logger::initFileSink()
{
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/basic-log.txt", true);
    file_sink->set_level(spdlog::level::trace);
    return file_sink;
}