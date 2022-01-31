//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#ifndef BSC_FILESORTERMAPPER_H
#define BSC_FILESORTERMAPPER_H
#include "FileSorterMapperMatcher.h"
#include "io/sorter/strategies/Strategies.h"
#include <algorithm>
#include <filesystem>
#include <io/file/FileInfo.h>
#include <optional>
#include <utility>
#include <vector>
#include <list>
namespace bsc {
    namespace fs = std::filesystem;

    struct FileSorterMappingResult {
        std::string pattern;
        FileSortingStrategies strategies;
    };

    /**
     * mapper that matches something about the file (mime type, filename) to destination path where it should be
     */
    class FileSorterMapper {

        struct MapperEntry {
            std::vector<FileSorterMapperMatcher> matcher;
            std::string pattern;
            int32_t priority;
            FileSortingStrategies strategies;
        };

        std::list<MapperEntry> patterns;
    public:
        std::optional<FileSorterMappingResult> map(const FileInfo& from);

        void addPattern(std::vector<FileSorterMapperMatcher> matcher, std::string pattern, FileSortingStrategies strategies, int32_t priority = 0) {
            MapperEntry entry{.matcher = std::move(matcher), .pattern = std::move(pattern), .priority = priority, .strategies = std::move(strategies)};
            patterns.insert(std::ranges::upper_bound(
                                    patterns,
                                    entry,
                                    [](auto& first, auto& second) { return first.priority > second.priority; }),
                            std::move(entry));
        }
        MatchPrecision accumulateMatchers(const std::vector<FileSorterMapperMatcher>& vector1, const FileInfo& info);
    };

}// namespace bsc

#endif