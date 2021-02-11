#include <utility>

//
// Created by Krzysztof Tulidowicz on 01.07.19.
//

#include "LoggerContext.h"

std::string bsc::LoggerContext::getInstance() { return instanceFetcher(); }
void bsc::LoggerContext::setInstanceFetcher(const std::function<std::string(void)>& instanceFetcher) {
    LoggerContext::instanceFetcher = instanceFetcher;
}
bsc::LoggerContext::LoggerContext() {
    instanceFetcher = []() { return ""; };
}
