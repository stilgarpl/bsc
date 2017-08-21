//
// Created by stilgar on 21.08.17.
//

#include "Logger.h"
#include "Poco/ConsoleChannel.h"

Logger::Logger(std::string name) : logger(Poco::Logger::get(name)) {

    Poco::ConsoleChannel *consoleChannel = new Poco::ConsoleChannel();
    // logger.setLevel(Poco::Message::Priority::PRIO_DEBUG);
    logger.setChannel(consoleChannel);

}

void Logger::error(std::string txt) {

    logger.error(txt);
}

void Logger::debug(std::string txt) {

    logger.debug(txt);
}

void Logger::info(std::string txt) {
    logger.information(txt);
}
