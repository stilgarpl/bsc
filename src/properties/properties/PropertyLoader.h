//
// Created by Krzysztof Tulidowicz on 29.01.2021.
//

#ifndef BSC_PROPERTYLOADER_H
#define BSC_PROPERTYLOADER_H

#include <context/context/Context.h>
namespace bsc {
    class PropertyLoader {
        Context::Ptr context;

    public:
        PropertyLoader();
        virtual ~PropertyLoader();
    };
}// namespace bsc

#endif