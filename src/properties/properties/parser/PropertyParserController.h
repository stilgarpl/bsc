//
// Created by Krzysztof Tulidowicz on 25.01.2021.
//

#ifndef BSC_PROPERTYPARSERCONTROLLER_H
#define BSC_PROPERTYPARSERCONTROLLER_H

#include "PropertyParser.h"
namespace bsc {
    class PropertyParserController {
        PropertyParser& propertyParser;
        const std::lock_guard<decltype(PropertyParser::mutex)> lockGuard;

    public:
        explicit PropertyParserController(PropertyParser& parser);
        PropertyParserController();
        virtual ~PropertyParserController();
        auto& parser() { return propertyParser; }
    };
}// namespace bsc

#endif