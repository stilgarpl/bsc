//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#ifndef BSC_FILESORTERMIMEMATCHER_H
#define BSC_FILESORTERMIMEMATCHER_H

#include "FileSorterMapperMatcher.h"
#include <io/file/MimeFileType.h>
#include <io/file/MimeFileTypeDecoder.h>
namespace bsc {

    namespace matchers{
        FileSorterMapperMatcher fileSorterMimeMatcher(const MimeFileType& mimeFileType);
    };

}// namespace bsc

#endif