//
// Created by Krzysztof Tulidowicz on 04.11.18.
//

#include <regex>
#include <core/log/Logger.h>
#include "TmpRule.h"
namespace bsc {
    JournalPathType TmpRule::transformToJournalFormat(fs::path path) {
//    LOGGER("tmprule ToJ " + path.string())
        std::string temp = fs::temp_directory_path().string();
        auto ret = std::regex_replace(path.string(), std::regex(temp), TEMP_PATTERN);
//    LOGGER("ret = " + ret)
        return ret;
    }

    std::filesystem::path TmpRule::transformFromJournalFormat(JournalPathType path) {
//    LOGGER("tmprule FroJ " + path.string())
        std::string temp = fs::temp_directory_path().string();
        auto ret = std::regex_replace(path, std::regex(TEMP_PATTERN), temp);
//    LOGGER("ret = " + ret)
        return ret;
    }
    TmpRule::TmpRule() : PathTransformerRule(tmpPriority) {}
    std::string TmpRule::getDefinition() { return "tmp"; }
}