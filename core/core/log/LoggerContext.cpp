#include <utility>

//
// Created by stilgar on 01.07.19.
//

#include "LoggerContext.h"


std::string LoggerContext::getInstance() {
    return instanceFetcher();
}

LoggerContext::LoggerContext(std::function<std::string()> instanceFetcher) : instanceFetcher(std::move(instanceFetcher)) {}
