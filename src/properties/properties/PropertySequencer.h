//
// Created by Krzysztof Tulidowicz on 21.06.2021.
//
#ifndef BSC_PROPERTYSEQUENCER_H
#define BSC_PROPERTYSEQUENCER_H

#include <properties/control/writer/PropertyWriter.h>

#include "Property.h"

namespace bsc {
    class PropertySequencer {
        PropertyWriter& writer;

    public:
        explicit PropertySequencer(PropertyWriter& writer) : writer(writer) {
        }

        template<IsProperty ... Args>
        void operator() (Args... args) {
            ((writer << args), ...);
        }
    };

    PropertyWriter& operator<<(PropertyWriter& writer, IsWritablePropertyClass auto const & writablePropertyClass)  {
        PropertySequencer sequencer(writer);
        writablePropertyClass.write(sequencer);
        return writer;
    }

    struct BasePropertyClass {
        virtual void write(PropertySequencer& sequencer) const = 0;
    };

}// namespace bsc

#endif