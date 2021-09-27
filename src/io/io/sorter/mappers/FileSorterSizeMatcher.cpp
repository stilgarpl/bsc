//
// Created by stilgar on 27.09.2021.
//

#include "FileSorterSizeMatcher.h"

namespace bsc::matchers {
    FileSorterMapperMatcher fileSorterSizeMatcher(std::uintmax_t size, MapperMatcherMode mode) {
        return [size, mode] (const FileInfo& fileInfo) {
            static ModeComparator<decltype(size)> modeComparator{};
            auto result = modeComparator.compare(fileInfo.fileSize, size, mode);
            if (result) {
                return MatchPrecision::perfect;
            } else {
                return MatchPrecision::none;
            }
        };
    }
}
