//
// Created by stilgar on 27.09.2021.
//

#include "FileSorterDateMatcher.h"

namespace bsc {
    namespace matchers{
        FileSorterMapperMatcher fileSorterDateMatcher(const FileDateValue& datetime) {
            return [datetime](const FileInfo& fileInfo){
                //@todo get rid of clock cast
                if (datetime.compare(date::clock_cast<std::chrono::system_clock>(fileInfo.fileModificationTime))) {
                    return MatchPrecision::perfect;
                } else {
                    return MatchPrecision::none;
                }
            };
        }
    };
}// namespace bsc
