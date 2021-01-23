//
// Created by Krzysztof Tulidowicz on 30.08.2019.
//

#ifndef BSC_FACTORYCONTEXT_H
#define BSC_FACTORYCONTEXT_H

#include "Factory.h"
#include <context/context/AutoContextSetter.h>
#include <core/uber/Uber.h>

namespace bsc {
    struct FactoryContext : public AutoContextSetter<FactoryContext>, public InitializerRegistry<FactoryContext> {
    private:
        Uber<Type> factories;

    public:
        template<typename ProducedObjectType,
                 typename FactorySpecialization = NoFactorySpecialization,
                 typename Traits                = FactoryTraits<ProducedObjectType, FactorySpecialization>>
        auto getFactory() {
            //@todo check if factories has this factory before returning? throw?
            return factories.get<typename Factory<ProducedObjectType, FactorySpecialization, Traits>::Ptr>().getType();
        }

        template<typename ProducedObjectType,
                 typename FactorySpecialization = NoFactorySpecialization,
                 typename Traits                = FactoryTraits<ProducedObjectType, FactorySpecialization>>
        bool hasFactory() {
            return factories.get<typename Factory<ProducedObjectType, FactorySpecialization, Traits>::Ptr>().getType() != nullptr;
        }

        template<typename ProducedObjectType,
                 typename FactorySpecialization = NoFactorySpecialization,
                 typename Traits                = FactoryTraits<ProducedObjectType, FactorySpecialization>>
        auto setFactory(typename Factory<ProducedObjectType, FactorySpecialization, Traits>::Ptr ptr) {
            factories.get<typename Factory<ProducedObjectType, FactorySpecialization, Traits>::Ptr>() = ptr;
            return ptr;
        }

        //@todo use/add concept Factory to limit types provided here to actual factories
        template<typename FactoryType, typename... FactoryConstructorArgs>
        auto addFactory(FactoryConstructorArgs&&... args) {
            typename Factory<typename FactoryType::ProducedObjectType,
                             typename FactoryType::FactorySpecialization,
                             typename FactoryType::Traits>::Ptr factoryPtr =
                    std::make_shared<FactoryType>(std::forward<FactoryConstructorArgs>(args)...);
            return setFactory<typename FactoryType::ProducedObjectType,
                              typename FactoryType::FactorySpecialization,
                              typename FactoryType::Traits>(factoryPtr);
        }

        FactoryContext() { initialize(); }
    };
}// namespace bsc

#endif// BSC_FACTORYCONTEXT_H
