//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_LOGGER_H
#define BASYCO_LOGGER_H

#include "Poco/Logger.h"
#include <string>
#include <mutex>
#include <filesystem>

namespace fs = std::filesystem;


#define LOGGER(x) Logger(fs::path(__FILE__).filename()).debug(__LINE__, x);
#define ERROR(x) Logger(fs::path(__FILE__).filename()).error(__LINE__, x);
#define SHOW(x) LOGGER(std::string(#x) +"="+std::to_string(x))
#define NODECONTEXTLOGGER(x) if (Context::getActiveContext()->get<NodeContext>()) Logger("["+Context::getActiveContext()->get<NodeContext>()->getNodeInfo().getNodeId() + "] " + __FILE__).debug(__LINE__,x);

class Logger {
private:

    Poco::Logger &logger;
    std::string loggerName;
public:

    std::mutex &getLock() {
        static std::mutex lock;
        return lock;
    }

    explicit Logger(std::string name);

    void debug(std::string txt);

    void debug(int line, const std::string &txt);

    void error(int line, const std::string &txt);
    void error(std::string txt);

    void info(std::string txt);
};


#endif //BASYCO_LOGGER_H