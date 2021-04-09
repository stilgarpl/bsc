//
// Created by Krzysztof Tulidowicz on 04.11.18.
//

#include "TmpRule.h"
#include <log/log/Logger.h>
#include <regex>
namespace bsc {
    JournalPathType TmpRule::transformToJournalFormat(fs::path path) {
        //    logger.debug("tmprule ToJ " + path.string());
        std::string temp = fs::temp_directory_path().string();
        auto ret = std::regex_replace(path.string(), std::regex(temp), TEMP_PATTERN);
        //    logger.debug("ret = " + ret);
        return ret;
    }

    std::filesystem::path TmpRule::transformFromJournalFormat(JournalPathType path) {
        //    logger.debug("tmprule FroJ " + path.string());
        std::string temp = fs::temp_directory_path().string();
        auto ret = std::regex_replace(path, std::regex(TEMP_PATTERN), temp);
        //    logger.debug("ret = " + ret);
        return ret;
    }
    TmpRule::TmpRule() : PathTransformerRule(tmpPriority) {}
    std::string TmpRule::getDefinition() { return "tmp"; }
}