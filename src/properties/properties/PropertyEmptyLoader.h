//
// Created by Krzysztof Tulidowicz on 10.08.2021.
//
#ifndef BSC_PROPERTYEMPTYLOADER_H
#define BSC_PROPERTYEMPTYLOADER_H

#include "PropertyLoader.h"
namespace bsc {
    class PropertyEmptyLoader final : public PropertyLoader{
    public:
        PropertyEmptyLoader();
    };
}// namespace bsc

#endif