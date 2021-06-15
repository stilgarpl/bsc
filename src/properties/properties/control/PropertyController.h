//
// Created by Krzysztof Tulidowicz on 25.01.2021.
//

#ifndef BSC_PROPERTYCONTROLLER_H
#define BSC_PROPERTYCONTROLLER_H

#include "properties/control/parser/PropertyParser.h"
namespace bsc {
    class PropertyController {
        PropertyParser& propertyParser;
        const std::lock_guard<decltype(PropertyParser::mutex)> lockGuard;

    public:
        //@todo this class can hold parser and emitter - constructor could take enum and construct it from the factory of supported parsers/emitters. Technically emitter could be a different type than parser, do I want to suport such case? And also -> when it has emitter, change to PropertyController
        explicit PropertyController(PropertyParser& parser);
        PropertyController();
        virtual ~PropertyController();
        auto& parser() { return propertyParser; }
    };


}// namespace bsc

#endif