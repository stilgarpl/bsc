//
// Created by stilgar on 21.08.17.
//

#include <iostream>
#include <spdlog/spdlog.h>
#include <p2p/node/context/NodeContext.h>
#include "Logger.h"
#include "Poco/ConsoleChannel.h"

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

void Logger::debug(int line, std::string txt) {

    std::lock_guard<std::mutex> g(getLock());
    static std::string::size_type nodeIdLength = 0;
//    std::cout << loggerName << " @ " << line << " : " << txt << std::endl;
//@todo optimize this call:
    if (Context::getActiveContext() != nullptr &&
        Context::getActiveContext()->get<NodeContext>()->getNodeInfo().getNodeId().length() > nodeIdLength) {
        nodeIdLength = Context::getActiveContext()->get<NodeContext>()->getNodeInfo().getNodeId().length();
    }
    if (Context::getActiveContext() != nullptr && Context::getActiveContext()->get<NodeContext>()) {
        spdlog::info("[{:<{}}] [{}:{}]: {}", Context::getActiveContext()->get<NodeContext>()->getNodeInfo().getNodeId(),
                     nodeIdLength,
                     loggerName, std::to_string(line), txt);
    } else {
        spdlog::info("[{}:{}]: {}", loggerName, std::to_string(line), txt);
    }

//    spdlog::info(txt);
}

void Logger::error(int line, std::string txt) {

    std::lock_guard<std::mutex> g(getLock());
//    std::cout << loggerName << " @ " << line << " : " << txt << std::endl;
    if (Context::getActiveContext() != nullptr && Context::getActiveContext()->get<NodeContext>()) {
        spdlog::error("[{:10}] [{}:{}]: {}", Context::getActiveContext()->get<NodeContext>()->getNodeInfo().getNodeId(),
                      loggerName, std::to_string(line), txt);
    } else {
        spdlog::error("[{}:{}]: {}", loggerName, std::to_string(line), txt);
    }

//    spdlog::info(txt);
}
