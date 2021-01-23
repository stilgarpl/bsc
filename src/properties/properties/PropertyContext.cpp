//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#include "PropertyContext.h"

namespace bsc {

    const std::shared_ptr<PropertyData>& PropertyContext::getPropertyData() const { return propertyData; }
    void PropertyContext::setPropertyData(const std::shared_ptr<PropertyData>& propertyData) {
        PropertyContext::propertyData = propertyData;
    }
}// namespace bsc
