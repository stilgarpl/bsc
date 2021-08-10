//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#include "FileSorterMapper.h"
#include <fmt/format.h>
namespace bsc {

    std::optional<std::string> FileSorterMapper::map(const FileInfo& from) {
        std::string_view bestMatch;
        MatchPrecision bestPrecision = MatchPrecision::none;
        auto bestPriority = std::numeric_limits<decltype(MapperEntry::priority)>::min();
        for (const auto& [matcher, pattern, priority] : patterns) {
            if(bestPrecision == MatchPrecision::perfect || priority < bestPriority ) {
                break;
            }
            auto matchResult = matcher(from);
            logger.info(fmt::format("Matching {} to {} with priority: {}", from.path.filename().string(), pattern, priority));
            switch (matchResult) {

                case MatchPrecision::none:
                    logger.info("No match");
                    continue;
                case MatchPrecision::perfect:// @todo it can't be better than perfect, we should break the loop immediately.
                default:
                    if (matchResult > bestPrecision) {
                        bestPrecision = matchResult;
                        bestMatch     = pattern;
                        bestPriority = priority;
                    }
                    logger.info("Match found");
            }
        }

        if (bestPrecision == MatchPrecision::none) {
            return std::nullopt;
        } else {
            return std::string(bestMatch);
        }
    }
}// namespace bsc