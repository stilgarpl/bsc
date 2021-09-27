//
// Created by stilgar on 27.09.2021.
//

#ifndef BSC_FILESORTERSIZEMATCHER_H
#define BSC_FILESORTERSIZEMATCHER_H

#include "FileSorterMapperMatcher.h"
namespace bsc {
    namespace matchers{
        FileSorterMapperMatcher fileSorterSizeMatcher(std::uintmax_t size, MapperMatcherMode mode);
    };
}// namespace bsc

#endif