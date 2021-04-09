//
// Created by Krzysztof Tulidowicz on 21.08.17.
//

#include "Logger.h"
#include "LoggerContext.h"
#include <iostream>
#include <spdlog/spdlog.h>

#include <filesystem>
namespace fs = std::filesystem;

bsc::Logger::Logger() {
}

namespace bsc {

    static std::string getFileName(const fs::path& p) {
        return p.filename();
    }

    static constexpr spdlog::level::level_enum mapLogLevel(Logger::LogLevel level) {
        switch (level) {
            case Logger::LogLevel::debug:
                return spdlog::level::level_enum::debug;
            case Logger::LogLevel::info:
                return spdlog::level::level_enum::info;
            case Logger::LogLevel::error:
                return spdlog::level::level_enum::err;
            default:
                return spdlog::level::level_enum::debug;
        }
    }

    void Logger::doLog(Logger::LogLevel level, const std::string& txt, std::experimental::source_location sourceLocation) {
        if (Context::hasActiveContext() && Context::getActiveContext()->get<LoggerContext>()->hasInstance()) {
            const auto& instance = Context::getActiveContext()->get<LoggerContext>()->getInstance();
            Context::getActiveContext()->get<LoggerContext>()->spdlogLogger->log(mapLogLevel(level),
                                                                                 "[{:<{}}] [{}:{}]: {}",
                                                                                 instance,
                                                                                 instance.size(),
                                                                                 getFileName(sourceLocation.file_name()),
                                                                                 std::to_string(sourceLocation.line()),
                                                                                 txt);
        } else {
            spdlog::log(mapLogLevel(level),
                        "[{}:{}]: {}",
                        getFileName(sourceLocation.file_name()),
                        std::to_string(sourceLocation.line()),
                        txt);
        }
    }

    void bsc::Logger::debug(const std::string& txt, std::experimental::source_location sourceLocation) {
        doLog(LogLevel::debug, txt, sourceLocation);
        //    spdlog::info(txt);
    }

}// namespace bsc
void bsc::Logger::info(const std::string& txt, std::experimental::source_location sourceLocation) {

    doLog(LogLevel ::info, txt, sourceLocation);
}

void bsc::Logger::error(const std::string& txt, std::experimental::source_location sourceLocation) {

    doLog(LogLevel ::error, txt, sourceLocation);
}
