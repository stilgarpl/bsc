//
// Created by stilgar on 30.08.2019.
//

#ifndef BASYCO_FACTORYCONTEXT_H
#define BASYCO_FACTORYCONTEXT_H


#include <core/uber/Uber.h>
#include "Factory.h"

struct FactoryContext {
private:
    Uber<Type> factories;
public:
    //@todo maybe allow for FactorySpecialization to affect SelectorType? I think it should
    template<typename ProducedObjectType, typename ...  FactorySpecialization>
    FactoryPtr<ProducedObjectType,FactorySpecialization...> getFactory(){

        return factories.get<FactoryPtr<ProducedObjectType,FactorySpecialization...>>();
    }

    template<typename ProducedObjectType, typename ...  FactorySpecialization>
    void setFactory(FactoryPtr<ProducedObjectType,FactorySpecialization...> ptr) {
        factories.get<FactoryPtr<ProducedObjectType,FactorySpecialization...>>() = ptr;
    }
};


#endif //BASYCO_FACTORYCONTEXT_H
