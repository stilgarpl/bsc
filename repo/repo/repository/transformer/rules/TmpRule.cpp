//
// Created by stilgar on 04.11.18.
//

#include <regex>
#include <p2p/log/Logger.h>
#include "TmpRule.h"

std::filesystem::path TmpRule::transformToJournalFormat(fs::path path) {
    LOGGER("tmprule ToJ " + path.string())
    std::string temp = fs::temp_directory_path();
    auto ret = std::regex_replace(path.string(), std::regex(temp), TEMP_PATTERN);
    LOGGER("ret = " + ret)
    return ret;
}

std::filesystem::path TmpRule::transformFromJournalFormat(fs::path path) {
    LOGGER("tmprule FroJ " + path.string())
    std::string temp = fs::temp_directory_path();
    auto ret = std::regex_replace(path.string(), std::regex(TEMP_PATTERN), temp);
    LOGGER("ret = " + ret)
    return ret;
}
