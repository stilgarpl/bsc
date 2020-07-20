//
// Created by Krzysztof Tulidowicz on 11.05.2020.
//

#include "FileSorterNameMatcher.h"

namespace bsc {
    MatchPrecision FileSorterNameMatcher::matches(const fs::path& path) {
        std::smatch match;
        if (std::regex_search(path.filename().string(), regex)) {
            return MatchPrecision ::perfect;
        } else {
            return MatchPrecision ::none;
        }
    }
}// namespace bsc
