//
// Created by stilgar on 21.08.17.
//

#include <iostream>
#include <spdlog/spdlog.h>
#include <core/context/Context.h>
#include "Logger.h"
#include "LoggerContext.h"


#include <utility>

bsc::Logger::Logger(std::string name) : loggerName(std::move(name)) {

//    Poco::ConsoleChannel *consoleChannel = new Poco::ConsoleChannel();
    // logger.setLevel(Poco::Message::Priority::PRIO_DEBUG);
//    logger.setChannel(consoleChannel);

    spdlog::set_pattern("[%T]%L:%v");
}

void bsc::Logger::error(const std::string& txt) {

//    logger.error(txt);
}

void bsc::Logger::debug(const std::string& txt) {
    std::lock_guard<std::mutex> g(getLock());
//    logger.debug(txt);
//    std::clog << loggerName << " : " << txt << std::endl;
    spdlog::debug(txt);
}

void bsc::Logger::info(const std::string& txt) {
//    logger.information(txt);
}

void bsc::Logger::debug(int line, const std::string& txt) {

    std::lock_guard<std::mutex> g(getLock());
    static std::string::size_type instanceLength = 0;
//@todo optimize this call:
    if (bsc::Context::hasActiveContext() && bsc::Context::getActiveContext()->has<bsc::LoggerContext>() &&
        bsc::Context::getActiveContext()->get<bsc::LoggerContext>().getInstance().size() > instanceLength) {
        instanceLength = bsc::Context::getActiveContext()->get<bsc::LoggerContext>().getInstance().size();
    }
    if (bsc::Context::hasActiveContext() && bsc::Context::getActiveContext()->has<bsc::LoggerContext>()) {
        spdlog::info("[{:<{}}] [{}:{}]: {}", bsc::Context::getActiveContext()->get<bsc::LoggerContext>().getInstance(),
                     instanceLength,
                     loggerName, std::to_string(line), txt);
    } else {
        spdlog::info("[{}:{}]: {}", loggerName, std::to_string(line), txt);
    }

//    spdlog::info(txt);
}

void bsc::Logger::error(int line, const std::string& txt) {

    std::lock_guard<std::mutex> g(getLock());
    static std::string::size_type instanceLength = 0;
//@todo optimize this call:
    if (bsc::Context::hasActiveContext() && bsc::Context::getActiveContext()->has<bsc::LoggerContext>() &&
        bsc::Context::getActiveContext()->get<bsc::LoggerContext>().getInstance().size() > instanceLength) {
        instanceLength = bsc::Context::getActiveContext()->get<bsc::LoggerContext>().getInstance().size();
    }
    if (bsc::Context::hasActiveContext() && bsc::Context::getActiveContext()->has<bsc::LoggerContext>()) {
        spdlog::error("[{:<{}}] [{}:{}]: {}", bsc::Context::getActiveContext()->get<bsc::LoggerContext>().getInstance(),
                      instanceLength,
                      loggerName, std::to_string(line), txt);
    } else {
        spdlog::error("[{}:{}]: {}", loggerName, std::to_string(line), txt);
    }
}
