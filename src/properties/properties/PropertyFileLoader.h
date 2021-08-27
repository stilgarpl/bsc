//
// Created by Krzysztof Tulidowicz on 28.01.2021.
//

#ifndef BSC_PROPERTYFILELOADER_H
#define BSC_PROPERTYFILELOADER_H

#include "PropertyDefinitions.h"
#include "PropertyLoader.h"
#include <context/context/Context.h>
namespace bsc {
    class PropertyFileLoader final : PropertyLoader {

    public:
        PropertyFileLoader(const fs::path& path, std::initializer_list<PropertySetting> settings = {});
    };
}// namespace bsc

#endif