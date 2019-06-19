//
// Created by stilgar on 21.08.17.
//

#include <iostream>
#include <spdlog/spdlog.h>
#include "Logger.h"
#include "Poco/ConsoleChannel.h"

Logger::Logger(std::string name) : logger(Poco::Logger::get(name)), loggerName(name) {

//    Poco::ConsoleChannel *consoleChannel = new Poco::ConsoleChannel();
    // logger.setLevel(Poco::Message::Priority::PRIO_DEBUG);
//    logger.setChannel(consoleChannel);

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
    std::cout << loggerName << " @ " << line << " : " << txt << std::endl;
//    spdlog::info(txt);
}
