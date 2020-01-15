#include <utility>

//
// Created by stilgar on 01.07.19.
//

#include "LoggerContext.h"


std::string bsc::LoggerContext::getInstance() {
    return instanceFetcher();
}

bsc::LoggerContext::LoggerContext(std::function<std::string()> instanceFetcher) : instanceFetcher(
        std::move(instanceFetcher)) {}
