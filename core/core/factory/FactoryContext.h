//
// Created by stilgar on 30.08.2019.
//

#ifndef BSC_FACTORYCONTEXT_H
#define BSC_FACTORYCONTEXT_H


#include "Factory.h"
#include <core/uber/Uber.h>


namespace bsc {
    struct FactoryContext {
    private:
        Uber<Type> factories;

    public:
        template<typename ProducedObjectType, typename FactorySpecialization = NoFactorySpecialization>
        FactoryPtr<ProducedObjectType, FactorySpecialization> getFactory() {
            return factories.get<FactoryPtr<ProducedObjectType, FactorySpecialization>>();
        }

        template<typename ProducedObjectType, typename FactorySpecialization = NoFactorySpecialization>
        auto setFactory(FactoryPtr<ProducedObjectType, FactorySpecialization> ptr) {
            factories.get<FactoryPtr<ProducedObjectType, FactorySpecialization>>() = ptr;
            return ptr;
        }

        //@todo use/add concept Factory to limit types provided here to actual factories
        template<typename FactoryType, typename... FactoryConstructorArgs>
        auto addFactory(FactoryConstructorArgs&&... args) {
            FactoryPtr<typename FactoryType::ProducedObjectType, typename FactoryType::FactorySpecialization> factoryPtr = std::make_shared<FactoryType>(std::forward<FactoryConstructorArgs>(args)...);
            return setFactory(factoryPtr);
        }
    };
}// namespace bsc


#endif//BSC_FACTORYCONTEXT_H
