//
// Created by stilgar on 04.11.18.
//

#include <regex>
#include "CustomRule.h"

CustomRule::CustomRule(const std::string &from, const std::string &to) : from(from), to(to) {}

std::filesystem::path CustomRule::transformToJournalFormat(fs::path path) {
    return std::regex_replace(path.string(), std::regex(from), to);
}

std::filesystem::path CustomRule::transformFromJournalFormat(fs::path path) {
    return std::regex_replace(path.string(), std::regex(to), from);
}
