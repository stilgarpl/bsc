//
// Created by stilgar on 13.10.2020.
//

#ifndef BSC_AUTOFACTORY_H
#define BSC_AUTOFACTORY_H

#include "Factory.h"
#include "FactoryContext.h"
#include <core/auto/AutoCaller.h>
namespace bsc {

    namespace detail {
        template<typename FactoryType>
        void registerFactoryInFactoryContext() {
            FactoryContext::registerInitializer([](FactoryContext& factoryContext) {
                using namespace std::string_literals;
                LOGGER("AUTO: Registering factory: "s + typeid(FactoryType).name())
                factoryContext.template addFactory<FactoryType>();
            });
        }
    }// namespace detail

    template<typename FactoryType,
             typename ProducedObjectType,
             typename FactorySpecialization = NoFactorySpecialization,
             typename Traits                = FactoryTraits<ProducedObjectType, FactorySpecialization>>
    class AutoFactory : public Factory<ProducedObjectType, FactorySpecialization, Traits>,
                        protected AutoCaller<FactoryType, detail::registerFactoryInFactoryContext<FactoryType>> {
    public:
        AutoFactory() : AutoCaller<FactoryType, detail::registerFactoryInFactoryContext<FactoryType>>({}){};
    };

}// namespace bsc

#endif