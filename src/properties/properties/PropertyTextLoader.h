//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#ifndef BSC_PROPERTYTEXTLOADER_H
#define BSC_PROPERTYTEXTLOADER_H

#include "PropertyLoader.h"
#include <context/context/Context.h>
#include <string>

namespace bsc {

    class PropertyTextLoader final : public PropertyLoader {

    public:
        PropertyTextLoader(const std::string& text);
    };
}// namespace bsc

#endif