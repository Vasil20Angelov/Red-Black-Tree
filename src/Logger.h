#pragma once
#include <memory>
#include <string>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"
using std::string;

class Logger {
    const int maxFileSize = 102'400;
    const int maxFilesNumber = 5;

public:
    Logger()
    {
        logger = std::make_shared<spdlog::logger>("basic_logger", initFileSink());
        logger->set_level(spdlog::level::trace);
    }

    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;
    ~Logger() = default;

    static std::shared_ptr<Logger> getInstance() {
        static std::shared_ptr<Logger> splg(new Logger);
        return splg;
    }

    void trace(const string& msg) {
        logger->trace(msg);
    }

    void debug(const string& msg) {
        logger->debug(msg);
    }

    void info(const string& msg) {
        logger->info(msg);
    }

    void warn(const string& msg) {
        logger->warn(msg);
    }

    void error(const string& msg) {
        logger->error(msg);
    }

    void critical(const string& msg) {
        logger->critical(msg);
    }

private:
    std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> initFileSink()
    {
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/basic-log.txt", maxFileSize, maxFilesNumber - 1);
        file_sink->set_level(spdlog::level::trace);
        return file_sink;
    }

    std::shared_ptr<spdlog::logger> logger;
    Logger* instance;
};
