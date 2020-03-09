//
// Created by stilgar on 04.11.18.
//

#include <regex>
#include "CustomRule.h"

namespace bsc {
    CustomRule::CustomRule(std::string from, std::string to) : from(std::move(from)), to(std::move(to)) {}

    JournalPathType CustomRule::transformToJournalFormat(fs::path path) {
        return std::regex_replace(path.string(), std::regex(from), to);
    }

    std::filesystem::path CustomRule::transformFromJournalFormat(JournalPathType path) {
        return std::regex_replace(path, std::regex(to), from);
    }

}