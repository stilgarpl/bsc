//
// Created by Krzysztof Tulidowicz on 27.08.2021.
//
#ifndef BSC_FILESORTERMAPPERPROPERTIES_H
#define BSC_FILESORTERMAPPERPROPERTIES_H

#include "Actions.h"
#include "io/sorter/mappers/FileSorterMapperMatcher.h"
#include <list>
#include <properties/Property.h>
#include <properties/PropertySequencer.h>

namespace bsc {

    enum class MapperType {
        regex,
        mime,
        size,
        date,
    };
    struct FileExistsProperties : BasePropertyClass {
        Property<"action", std::string> action{"rename"};
        Property<"pattern", std::string> pattern{"({})"};
        void write(PropertySequencer& sequencer) const override;
        virtual ~FileExistsProperties() = default;
    };

    struct ActionProperties : BasePropertyClass {
        Property<"sort", SortAction> sort{SortAction::copy};
        //@todo maybe convert those strings to enums? "continue" action stands in the way - maybe rename to keepgoing
        Property<"error", std::string> error{"continue"};
        Property<"fileExists", FileExistsProperties> fileExists{};
        void write(PropertySequencer& sequencer) const override;
        virtual ~ActionProperties() = default;
    };

    struct MatcherProperties : BasePropertyClass {
        Property<"type", MapperType> type;
        Property<"match", std::string> match{};
        void write(PropertySequencer& sequencer) const override;
        virtual ~MatcherProperties() = default;
    };

    struct FileSorterMapperProperties : BasePropertyClass {
        Property<"matcher", std::list<MatcherProperties>> matchers;
        Property<"pattern", std::string> pattern{};
        Property<"on", ActionProperties> actions{};
        void write(PropertySequencer& sequencer) const override;
        virtual ~FileSorterMapperProperties() = default;
    };

    struct FileSorterProperties : BasePropertyClass {
        Property<"rules", std::list<FileSorterMapperProperties>> rules;
        Property<"version", std::string> version{"0.1"};
        FileSorterMapperProperties& addOrUpdateRule(MapperType mapperType,
                                                    std::string match,
                                                    std::string pattern,
                                                    SortAction sort,
                                                    std::string errorAction,
                                                    std::string fileExists,
                                                    std::string renamePattern);
        void write(PropertySequencer& sequencer) const override;
        virtual ~FileSorterProperties() = default;
    };
}// namespace bsc

#endif