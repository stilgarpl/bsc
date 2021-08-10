//
// Created by Krzysztof Tulidowicz on 11.05.2020.
//

#ifndef BSC_FILESORTERNAMEMATCHER_H
#define BSC_FILESORTERNAMEMATCHER_H
#include "FileSorterMapperMatcher.h"
#include <regex>
#include <string>

namespace bsc {

    namespace matchers {
        FileSorterMapperMatcher fileSorterNameMatcher(const std::string& regexPattern);
    };

}// namespace bsc

#endif