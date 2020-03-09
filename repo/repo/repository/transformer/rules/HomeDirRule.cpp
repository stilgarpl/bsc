//
// Created by stilgar on 04.11.18.
//

#include <regex>
#include "HomeDirRule.h"
namespace bsc {
    JournalPathType HomeDirRule::transformToJournalFormat(fs::path path) {
        return std::regex_replace(path.string(), std::regex("^" + HOME_DIR), HOME_PATTERN);
    }

    std::filesystem::path HomeDirRule::transformFromJournalFormat(JournalPathType path) {
        return std::regex_replace(path, std::regex(HOME_PATTERN), HOME_DIR);
    }
}