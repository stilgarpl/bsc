//
// Created by Krzysztof Tulidowicz on 11.05.2020.
//

#ifndef BSC_FILESORTERNAMEMATCHER_H
#define BSC_FILESORTERNAMEMATCHER_H
#include "FileSorterMapperMatcher.h"
#include <regex>
#include <string>

namespace bsc {
    class FileSorterNameMatcher final : public FileSorterMapperMatcher {
        std::regex regex;

    public:
        explicit FileSorterNameMatcher(const std::string& regexPattern) : regex(regexPattern) {}
        MatchPrecision matches(const fs::path& path) override;
        ~FileSorterNameMatcher() override = default;
    };
}// namespace bsc

#endif