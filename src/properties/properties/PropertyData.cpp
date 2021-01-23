//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#include "PropertyData.h"

namespace bsc {

    PropertyData::PropertyData(const YAML::Node& node) : node(node) {}
    InvalidPropertyKeyException::InvalidPropertyKeyException(const std::string& arg) : domain_error(arg) {}
    InvalidPropertyTypeException::InvalidPropertyTypeException(const std::string& arg) : domain_error(arg) {}
}// namespace bsc
