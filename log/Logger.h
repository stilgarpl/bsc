//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_LOGGER_H
#define BASYCO_LOGGER_H

#include "Poco/Logger.h"
#include <string>

class Logger {
private:
    Poco::Logger &logger;
public:

    Logger(std::string name);

    void debug(std::string txt);

    void error(std::string txt);

    void info(std::string txt);
};


#endif //BASYCO_LOGGER_H
