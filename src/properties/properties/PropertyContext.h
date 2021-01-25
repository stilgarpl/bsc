//
// Created by Krzysztof Tulidowicz on 20.01.2021.
//

#ifndef BSC_PROPERTYCONTEXT_H
#define BSC_PROPERTYCONTEXT_H

#include <context/context/AutoContextSetter.h>
#include <properties/parser/PropertyParser.h>
#include <utility>
namespace bsc {

    class PropertiesNotLoaded : public std::domain_error {
    public:
        explicit PropertiesNotLoaded(const std::string& arg);
    };

    class PropertyContext : public AutoContextSetter<PropertyContext> {
        std::unique_ptr<PropertyParser> propertyParser = nullptr;

    public:
        PropertyParser& getPropertyParser() {
            if (propertyParser == nullptr) {
                throw PropertiesNotLoaded("Property parser is not set");
            }
            return *propertyParser;
        }

        template<IsPropertyParser ParserType, typename... Args>
        void setPropertyParser(Args&&... args) {
            propertyParser = std::make_unique<ParserType>(std::forward<Args...>(args...));
        }
    };
}// namespace bsc

#endif