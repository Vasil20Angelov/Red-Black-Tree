#pragma once
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

class Logger {
public:
    Logger();
    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;
    ~Logger() = default;

    spdlog::logger* operator->();

private:
    std::shared_ptr<spdlog::sinks::basic_file_sink_mt> initFileSink();
    std::shared_ptr<spdlog::logger> logger;

    // TODO: Rotation files
	//auto my_logger = spdlog::basic_logger_mt("file_logger", "logs/basic-log.txt", false);
    //auto fileLogger = spdlog::rotating_logger_mt("file_logger", "logs/mylogfile", 1024 * 5, 3);
};

extern Logger fileLogger;