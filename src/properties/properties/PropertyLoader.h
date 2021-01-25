//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#ifndef BSC_PROPERTYLOADER_H
#define BSC_PROPERTYLOADER_H

#include <string>

namespace bsc {

    class PropertyLoader {

    public:
        //@todo refactor this into more usable interface
        //        void loadToContext(const fs::path& path);
        static void parseToContext(const std::string& text);
    };
}// namespace bsc

#endif