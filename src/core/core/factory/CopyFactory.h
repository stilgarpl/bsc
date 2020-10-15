//
// Created by Krzysztof Tulidowicz on 11.05.2020.
//

#ifndef BSC_COPYFACTORY_H
#define BSC_COPYFACTORY_H

#include "AutoFactory.h"
#include "Factory.h"
#include <map>
#include <set>
namespace bsc {

    //@todo replace with Factory::InvalidSelector
    class CopyFactoryNoMoldException : public std::domain_error {
    public:
        CopyFactoryNoMoldException(const std::string& arg) : domain_error(arg) {}
    };

    template<typename ProducedObjectType, typename FactorySpecialization = NoFactorySpecialization>
    class CopyFactory
        : public AutoFactory<CopyFactory<ProducedObjectType, FactorySpecialization>, ProducedObjectType, FactorySpecialization> {
        using SelectorType = typename Factory<ProducedObjectType, FactorySpecialization>::SelectorType;

    private:
        std::map<SelectorType, ProducedObjectType> molds{};

    public:
        ProducedObjectType create(const SelectorType& selector) const override {
            if (molds.contains(selector)) {
                return molds.at(selector);
            } else {
                using namespace std::string_literals;
                //@todo this assumes selector is string. fix for other types
                throw CopyFactoryNoMoldException("Mold missing for selector: ");
            }
        }
        void addMold(const SelectorType& selector, ProducedObjectType object) { molds[selector] = object; }
        std::set<SelectorType> getMolds() {
            std::set<SelectorType> keys;
            for (const auto& [key, value] : molds) {
                keys.emplace(key);
            }
            return keys;
        }
    };
}// namespace bsc

#endif