//
// Created by stilgar on 30.08.2019.
//

#ifndef BASYCO_FACTORY_H
#define BASYCO_FACTORY_H


#include <memory>

template<typename ProducedObjectType,typename ...  FactorySpecialization>
struct FactoryTraits {
    using SelectorType = std::string;
};

template<typename ProducedObjectType,typename ...  FactorySpecialization>
struct Factory {
    using SelectorType = typename FactoryTraits<ProducedObjectType,FactorySpecialization...>::SelectorType;

    virtual ProducedObjectType create(const SelectorType& selector)=0;
};

template<typename ProducedObjectType,typename ...  FactorySpecialization>
using FactoryPtr = std::shared_ptr<Factory<ProducedObjectType,FactorySpecialization...>>;



#endif //BASYCO_FACTORY_H
