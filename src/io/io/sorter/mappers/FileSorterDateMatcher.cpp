//
// Created by stilgar on 27.09.2021.
//

#include "FileSorterDateMatcher.h"

namespace bsc {
    namespace matchers{
        FileSorterMapperMatcher fileSorterDateMatcher(fs::file_time_type datetime, MapperMatcherMode mode) {
            return [datetime, mode](const FileInfo& fileInfo){
                static ModeComparator<decltype(datetime)> modeComparator{};
                auto result = modeComparator.compare(fileInfo.fileModificationTime, datetime, mode);
                if (result) {
                    return MatchPrecision::perfect;
                } else {
                    return MatchPrecision::none;
                }
            };
        }
    };
}// namespace bsc
