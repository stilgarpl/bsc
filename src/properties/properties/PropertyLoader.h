//
// Created by Krzysztof Tulidowicz on 29.01.2021.
//

#ifndef BSC_PROPERTYLOADER_H
#define BSC_PROPERTYLOADER_H

#include "PropertySetting.h"
#include <context/context/Context.h>
namespace bsc {
    class PropertyLoader {
        Context::Ptr context;

    public:
        PropertyLoader();
        virtual ~PropertyLoader();
        void enableOptions(std::initializer_list<PropertySetting> settings);
        void disableOptions(std::initializer_list<PropertySetting> settings);

    };
}// namespace bsc

#endif