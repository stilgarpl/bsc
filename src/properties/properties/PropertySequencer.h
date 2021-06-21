//
// Created by Krzysztof Tulidowicz on 21.06.2021.
//
#ifndef BSC_PROPERTYSEQUENCER_H
#define BSC_PROPERTYSEQUENCER_H

#include "Property.h"

namespace bsc {
    class PropertySequencer {
        PropertyWriter& writer;

    public:
        explicit PropertySequencer(PropertyWriter& writer) : writer(writer) {
        }

        template<IsProperty ... Args>
        void operator() (Args... args) {
            (args.write(writer),...);
        }
    };
}// namespace bsc

#endif