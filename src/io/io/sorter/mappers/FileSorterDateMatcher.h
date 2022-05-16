//
// Created by stilgar on 27.09.2021.
//

#ifndef BSC_FILESORTERDATEMATCHER_H
#define BSC_FILESORTERDATEMATCHER_H

#include "FileSorterMapperMatcher.h"
#include "parser/value/ParametrizedValue.h"
#include "parser/value/SiAffix.h"
#include "parser/value/StandardAffixes.h"
namespace bsc {

    using FileDateValue =  ParametrizedValue<std::chrono::system_clock::time_point , affixes::RelationAffix, affixes::NoAffix>;

    namespace matchers{
        FileSorterMapperMatcher fileSorterDateMatcher(const FileDateValue& dateValue);
    };
}// namespace bsc

#endif