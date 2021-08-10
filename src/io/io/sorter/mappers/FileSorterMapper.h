//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#ifndef BSC_FILESORTERMAPPER_H
#define BSC_FILESORTERMAPPER_H
#include "FileSorterMapperMatcher.h"
#include <algorithm>
#include <filesystem>
#include <io/file/FileInfo.h>
#include <optional>
#include <utility>
#include <vector>
namespace bsc {
    namespace fs = std::filesystem;
    /**
     * mapper that matches something about the file (mime type, filename) to destination path where it should be
     */
    class FileSorterMapper {

        struct MapperEntry {
            FileSorterMapperMatcher matcher;
            std::string pattern;
            int32_t priority;
        };

        std::vector<MapperEntry> patterns;
    public:
        std::optional<std::string> map(const FileInfo& from);

        void addPattern(FileSorterMapperMatcher matcher, std::string pattern, int32_t priority = 0) {
            MapperEntry entry{.matcher = std::move(matcher), .pattern = std::move(pattern), .priority = priority};
            patterns.insert(std::ranges::upper_bound(
                                    patterns,
                                    entry,
                                    [](auto& first, auto& second) { return first.priority > second.priority; }),
                            std::move(entry));
        }
    };

}// namespace bsc

#endif