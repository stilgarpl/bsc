//
// Created by stilgar on 20.04.2020.
//

#include "FileSorterMapper.h"

namespace bsc {

    std::optional<std::string> FileSorterMapper::map(const fs::path& from) {
        std::string_view bestMatch;
        MatchPrecision bestPrecision = MatchPrecision::none;
        for (const auto& [matcher, pattern] : patterns) {
            //@todo convert to std (max?) algorithm
            auto matchResult = matcher->matches(from);
            switch (matchResult) {

                case MatchPrecision::none:
                    continue;
                    break;
                case MatchPrecision::perfect:// @todo it can't be better than perfect, we should break the loop
                                             // immediately.
                default:
                    if (matchResult > bestPrecision) {
                        bestPrecision = matchResult;
                        bestMatch     = pattern;
                    }
            }
        }

        if (bestPrecision == MatchPrecision::none) {
            return std::nullopt;
        } else {
            return std::string(bestMatch);
        }
    }
}// namespace bsc