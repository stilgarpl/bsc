//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#ifndef BSC_PROPERTYLOADER_H
#define BSC_PROPERTYLOADER_H

#include "PropertyData.h"

namespace bsc {

    class PropertyLoader {

    public:
        static std::shared_ptr<PropertyData> load(const fs::path& path);
        static std::shared_ptr<PropertyData> parse(const std::string& text);

        //@todo refactor this into more usable interface
        void loadToContext(const fs::path& path);
        static void parseToContext(const std::string& text);
    };
}// namespace bsc

#endif