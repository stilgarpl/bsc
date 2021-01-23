//
// Created by Krzysztof Tulidowicz on 21.01.2021.
//

#ifndef BSC_PROPERTYDEFINITIONS_H
#define BSC_PROPERTYDEFINITIONS_H
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace bsc {
    using PropertyIdType     = std::string;
    using PropertyIdSequence = std::vector<PropertyIdType>;
    using PropertyValueType  = std::string;

    constexpr char propertyDelimiter = '.';
}// namespace bsc

#endif// BSC_PROPERTYDEFINITIONS_H
