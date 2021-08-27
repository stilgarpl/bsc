//
// Created by Krzysztof Tulidowicz on 27.08.2021.
//
#ifndef BSC_FILESORTERMAPPERPROPERTIES_H
#define BSC_FILESORTERMAPPERPROPERTIES_H

#include "Actions.h"
#include <list>
#include <properties/Property.h>
#include <properties/PropertySequencer.h>

namespace bsc {

    enum class MapperType {
        regex,
        mime,
    };

    struct ActionProperties : BasePropertyClass {
        Property<"sort",SortAction> sort{SortAction::copy};
        //@todo maybe convert those strings to enums? "continue" action stands in the way - maybe rename to keepgoing
        Property<"error",std::string> error {"continue"};
        Property<"fileExists",std::string> fileExists{"rename"};
        void write(PropertySequencer& sequencer) const override;
    };

    struct FileSorterMapperProperties : BasePropertyClass{
        Property<"type", MapperType> type;
        Property<"match", std::string> match{};
        Property<"pattern", std::string> pattern{};
        Property<"on", ActionProperties> actions{};
        void write(PropertySequencer& sequencer) const override;
    };

    struct FileSorterProperties : BasePropertyClass{
        Property<"rules", std::list<FileSorterMapperProperties>> rules{};
        Property<"version", std::string > version{"0.1"};
        void addOrUpdateRule(MapperType mapper, std::string match, std::string pattern, SortAction sort, std::string errorAction, std::string fileExists);
        void write(PropertySequencer& sequencer) const override;
    };
}// namespace bsc

#endif