//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#include "FileSorterMapper.h"
#include <fmt/format.h>
namespace bsc {

    std::optional<std::string> FileSorterMapper::map(const FileInfo& from) {
        std::string_view bestMatch;
        MatchPrecision bestPrecision = MatchPrecision::none;
        for (const auto& [matcher, pattern, priority] : patterns) {
            //@todo convert to std (max?) algorithm
            auto matchResult = matcher->matches(from.path);
            logger.info(fmt::format("Matching {} to {} with priority: {}", from.path.filename().string(), pattern, priority));
            switch (matchResult) {

                case MatchPrecision::none:
                    logger.info("No match");
                    continue;
                case MatchPrecision::perfect:// @todo it can't be better than perfect, we should break the loop// immediately.
                default:
                    if (matchResult > bestPrecision) {
                        bestPrecision = matchResult;
                        bestMatch     = pattern;
                    }
                    logger.info("Match found");
                    break;
            }
        }

        if (bestPrecision == MatchPrecision::none) {
            return std::nullopt;
        } else {
            return std::string(bestMatch);
        }
    }
}// namespace bsc