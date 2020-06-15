//
// Created by stilgar on 09.06.2020.
//

#ifndef BSC_PARSINGFACTORY_H
#define BSC_PARSINGFACTORY_H

#include "Factory.h"
#include <parser/cast/templateCast.h>
namespace bsc {

    //@todo this has the requirement of traits::argumentType = std::vector<std::string>>. try to find a way to enforce
    //it, maybe through concepts.
    template<typename ProducedObjectType, typename FactorySpecialization = NoFactorySpecialization>
    class ParsingFactory : public Factory<ProducedObjectType, FactorySpecialization> {
        using SelectorType = typename Factory<ProducedObjectType, FactorySpecialization>::SelectorType;
        using ArgumentType = typename Factory<ProducedObjectType, FactorySpecialization>::ArgumentType;
        using Creator      = std::function<ProducedObjectType(const ArgumentType&)>;
        std::map<SelectorType, Creator> creatorRegistry;

    public:
        ProducedObjectType create(const SelectorType& selector, const ArgumentType& argument) const override {
            if (creatorRegistry.contains(selector)) {
                return creatorRegistry.at(selector)(argument);
            }
            if constexpr (std::is_convertible_v<SelectorType, std::string>) {
                //@todo right error message
                throw FactoryInvalidSelector(selector);
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

        template<typename... Args>
        void registerCreator(const SelectorType& selector, ProducedObjectType (*f)(Args... args)) {
            creatorRegistry[selector] = [f](const ArgumentType& arguments) { return runFunction(f, arguments); };
        }
    };
}// namespace bsc

#endif