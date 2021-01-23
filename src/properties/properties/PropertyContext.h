//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#ifndef BSC_PROPERTYCONTEXT_H
#define BSC_PROPERTYCONTEXT_H

#include "PropertyData.h"
#include <context/context/AutoContextSetter.h>
namespace bsc {
    class PropertyContext : public AutoContextSetter<PropertyContext> {

        std::shared_ptr<PropertyData> propertyData;

    public:
        const std::shared_ptr<PropertyData>& getPropertyData() const;
        void setPropertyData(const std::shared_ptr<PropertyData>& propertyData);
    };
}// namespace bsc

#endif