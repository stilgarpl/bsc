#include <utility>

//
// Created by Krzysztof Tulidowicz on 01.07.19.
//

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "LoggerContext.h"

std::string bsc::LoggerContext::getInstance() {
    return instanceFetcher();
}
void bsc::LoggerContext::setInstanceFetcher(const std::function<std::string(void)>& instanceFetcher) {
    LoggerContext::instanceFetcher = instanceFetcher;
}

bool bsc::LoggerContext::hasInstance() {
    return instanceFetcher != nullptr;
}
bsc::LoggerContext::LoggerContext() {
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_level(spdlog::level::debug);
    consoleSink->set_pattern("[%T]%L:%v");

    //    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt", true);
    //    file_sink->set_level(spdlog::level::trace);

    spdlogLogger = std::make_shared<spdlog::logger>("multi_sink", consoleSink);
    spdlogLogger->set_level(spdlog::level::trace);
}
