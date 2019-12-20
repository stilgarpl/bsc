//
// Created by stilgar on 01.07.19.
//

#ifndef BASYCO_LOGGERCONTEXT_H
#define BASYCO_LOGGERCONTEXT_H


#include <functional>
#include <optional>
#include <string>

class LoggerContext {

private:
    std::function<std::string(void)> instanceFetcher;

public:
    LoggerContext(std::function<std::string()> instanceFetcher);

    std::string getInstance();

};


#endif //BASYCO_LOGGERCONTEXT_H
