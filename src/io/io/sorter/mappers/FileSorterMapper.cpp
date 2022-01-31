//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#include "FileSorterMapper.h"
#include <fmt/format.h>
#include <ranges>
namespace bsc {

    std::optional<FileSorterMappingResult> FileSorterMapper::map(const FileInfo& from) {
        //        std::string_view bestMatch;
        const MapperEntry* bestMatch = nullptr;
        MatchPrecision bestPrecision = MatchPrecision::none;
        auto bestPriority            = std::numeric_limits<decltype(MapperEntry::priority)>::min();
        for (const auto& entry : patterns) {
            bool foundBest = false;
            if (entry.priority < bestPriority) {
                break;
            }
            auto matchResult = accumulateMatchers(entry.matcher, from);
            logger.info(fmt::format("Matching {} to {} with priority: {}", from.path.filename().string(), entry.pattern, entry.priority));
            switch (matchResult) {

                case MatchPrecision::none:
                    logger.info("No match");
                    continue;

                case MatchPrecision::perfect:
                    foundBest = true;
                    [[fallthrough]];
                default:
                    if (matchResult > bestPrecision) {
                        bestPrecision = matchResult;
                        bestMatch     = &entry;
                        bestPriority  = entry.priority;
                    }
                    logger.info("Match found");
            }
            if (foundBest) {
                break;
            }
        }

        if (bestPrecision == MatchPrecision::none || bestMatch == nullptr) {
            return std::nullopt;
        } else {
            return FileSorterMappingResult{.pattern = std::string(bestMatch->pattern), .strategies = bestMatch->strategies};
        }
    }
    MatchPrecision FileSorterMapper::accumulateMatchers(const std::vector<FileSorterMapperMatcher>& matchers, const FileInfo& info) {
        MatchPrecision result = MatchPrecision::perfect;
        for (const auto& matcher : matchers) {
            result = std::min(matcher(info), result);
        }
        return result;
    }
}// namespace bsc