//
// Created by stilgar on 21.08.17.
//

#include <iostream>
#include <spdlog/spdlog.h>
#include <core/context/Context.h>
#include "Logger.h"
#include "Poco/ConsoleChannel.h"
#include "LoggerContext.h"

Logger::Logger(std::string name) : logger(Poco::Logger::get(name)), loggerName(name) {

//    Poco::ConsoleChannel *consoleChannel = new Poco::ConsoleChannel();
    // logger.setLevel(Poco::Message::Priority::PRIO_DEBUG);
//    logger.setChannel(consoleChannel);

    spdlog::set_pattern("[%T]%L:%v");
}

void Logger::error(std::string txt) {

//    logger.error(txt);
}

void Logger::debug(std::string txt) {
    std::lock_guard<std::mutex> g(getLock());
//    logger.debug(txt);
//    std::clog << loggerName << " : " << txt << std::endl;
    spdlog::debug(txt);
}

void Logger::info(std::string txt) {
//    logger.information(txt);
}

void Logger::debug(int line, const std::string &txt) {

    std::lock_guard<std::mutex> g(getLock());
    static std::string::size_type instanceLength = 0;
//@todo optimize this call:
    if (Context::getActiveContext() != nullptr && Context::getActiveContext()->get<LoggerContext>() &&
        Context::getActiveContext()->get<LoggerContext>()->getInstance().size() > instanceLength) {
        instanceLength = Context::getActiveContext()->get<LoggerContext>()->getInstance().size();
    }
    if (Context::getActiveContext() != nullptr && Context::getActiveContext()->get<LoggerContext>()) {
        spdlog::info("[{:<{}}] [{}:{}]: {}", Context::getActiveContext()->get<LoggerContext>()->getInstance(),
                     instanceLength,
                     loggerName, std::to_string(line), txt);
    } else {
        spdlog::info("[{}:{}]: {}", loggerName, std::to_string(line), txt);
    }

//    spdlog::info(txt);
}

void Logger::error(int line, const std::string &txt) {

    std::lock_guard<std::mutex> g(getLock());
    static std::string::size_type instanceLength = 0;
//@todo optimize this call:
    if (Context::getActiveContext() != nullptr && Context::getActiveContext()->get<LoggerContext>() &&
        Context::getActiveContext()->get<LoggerContext>()->getInstance().size() > instanceLength) {
        instanceLength = Context::getActiveContext()->get<LoggerContext>()->getInstance().size();
    }
    if (Context::getActiveContext() != nullptr && Context::getActiveContext()->get<LoggerContext>()) {
        spdlog::error("[{:<{}}] [{}:{}]: {}", Context::getActiveContext()->get<LoggerContext>()->getInstance(),
                      instanceLength,
                      loggerName, std::to_string(line), txt);
    } else {
        spdlog::error("[{}:{}]: {}", loggerName, std::to_string(line), txt);
    }
}
