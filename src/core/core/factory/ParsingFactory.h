//
// Created by Krzysztof Tulidowicz on 09.06.2020.
//

#ifndef BSC_PARSINGFACTORY_H
#define BSC_PARSINGFACTORY_H

#include "AutoFactory.h"
#include "Factory.h"
#include <map>
#include <parser/cast/templateCast.h>
#include <set>
#include <vector>
#include <magic_enum.hpp>
namespace bsc {

    //@todo well, ParsingFactory does not have to be an auto-factory. It should be an option.
    template<typename ProducedObjectType, typename FactorySpecialization = NoFactorySpecialization>
    class ParsingFactory : public TemplateAutoFactory<ParsingFactory, ProducedObjectType, FactorySpecialization> {

        using Base         = TemplateAutoFactory<ParsingFactory, ProducedObjectType, FactorySpecialization>;
        using SelectorType = typename Base::SelectorType;
        //@todo maybe required type can be relaxed to any container of strings
        using RequiredArgumentType = std::vector<std::string>;
    public:
        using ArgumentType         = typename Base::ArgumentType;
    private:
        static_assert(std::is_same_v<ArgumentType, RequiredArgumentType>,
                      "ParsingFactory requires argument type to be of type std::vector<std::string>");
        using Creator = std::function<ProducedObjectType(const ArgumentType&)>;
        std::map<SelectorType, Creator> creatorRegistry;

    public:
        ProducedObjectType create(const SelectorType& selector, const ArgumentType& argument = {}) const override {
            if (creatorRegistry.contains(selector)) {
                return creatorRegistry.at(selector)(argument);
            }
            if constexpr (std::is_convertible_v<SelectorType, std::string>) {
                //@todo right error message
                throw FactoryInvalidSelector(selector);
            } else if constexpr (std::is_enum_v<SelectorType>) {
                throw FactoryInvalidSelector(std::string(magic_enum::enum_name(selector)));
            } else {
                throw FactoryInvalidSelector(std::to_string(selector));
            }
        }

        void registerCreator(const SelectorType& selector, const ProducedObjectType& object) {
            creatorRegistry[selector] = [object = object](const ArgumentType&) { return object; };
        }

        void registerCreator(const SelectorType& selector, const Creator& creator) {
            creatorRegistry[selector] = creator;
        }

        //@todo add a way to pass any callable (like lambda with captures) here, not just function pointer
        template<typename... Args>
        void registerCreator(const SelectorType& selector, ProducedObjectType (*f)(Args... args)) {
            creatorRegistry[selector] = [f](const ArgumentType& arguments) { return runFunction(f, arguments); };
        }

        std::set<SelectorType> getSelectors() const {
            // building this set every time this is called maybe not the most efficient, but the alternative is storing
            // it along with the map
            //@todo C++20 convert to ranges
            std::set<SelectorType> result;
            for (const auto& [key, value] : creatorRegistry) {
                result.insert(key);
            }
            return result;
        }
    };
}// namespace bsc

#endif