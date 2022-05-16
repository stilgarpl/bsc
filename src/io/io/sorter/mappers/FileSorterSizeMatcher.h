//
// Created by stilgar on 27.09.2021.
//

#ifndef BSC_FILESORTERSIZEMATCHER_H
#define BSC_FILESORTERSIZEMATCHER_H

#include "FileSorterMapperMatcher.h"
#include "parser/value/ParametrizedValue.h"
#include "parser/value/SiAffix.h"
#include "parser/value/StandardAffixes.h"
namespace bsc {

    using SizeValue =  ParametrizedValue<std::uintmax_t, affixes::RelationAffix, affixes::SiEnumPrefix>;

    namespace matchers{
        FileSorterMapperMatcher fileSorterSizeMatcher(const SizeValue& size) {
            return [size] (const FileInfo& fileInfo) {
                if (size.compare(fileInfo.fileSize)) {
                    return MatchPrecision::perfect;
                } else {
                    return MatchPrecision::none;
                }
            };
        }
    };
}// namespace bsc

#endif