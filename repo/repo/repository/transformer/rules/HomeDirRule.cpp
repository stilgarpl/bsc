//
// Created by stilgar on 04.11.18.
//

#include <regex>
#include "HomeDirRule.h"
namespace bsc {
    JournalPathType HomeDirRule::transformToJournalFormat(fs::path path) {
        return std::regex_replace(path.string(), std::regex("^" + homeDir), homePattern);
    }

    std::filesystem::path HomeDirRule::transformFromJournalFormat(JournalPathType path) {
        return std::regex_replace(path, std::regex(homePattern), homeDir);
    }
    HomeDirRule::HomeDirRule(const std::string& homeDir)
        : PathTransformerRule(homeDirPriority), homeDir(homeDir) {}
    std::string HomeDirRule::getDefinition() { return homeDir; }
    //getenv("HOME")
}