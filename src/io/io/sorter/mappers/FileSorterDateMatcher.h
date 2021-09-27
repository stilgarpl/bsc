//
// Created by stilgar on 27.09.2021.
//

#ifndef BSC_FILESORTERDATEMATCHER_H
#define BSC_FILESORTERDATEMATCHER_H

#include "FileSorterMapperMatcher.h"
namespace bsc {
    namespace matchers{
        FileSorterMapperMatcher fileSorterDateMatcher(fs::file_time_type, MapperMatcherMode mode);
    };
}// namespace bsc

#endif