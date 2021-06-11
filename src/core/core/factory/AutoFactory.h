//
// Created by stilgar on 13.10.2020.
//

#ifndef BSC_AUTOFACTORY_H
#define BSC_AUTOFACTORY_H

#include "Factory.h"
#include "FactoryContext.h"
#include <context/auto/AutoCaller.h>
#include <log/log/Logger.h>
namespace bsc {

    namespace detail {
        template<typename FactoryType>
        void registerFactoryInFactoryContext() {
            FactoryContext::registerInitializer([](FactoryContext& factoryContext) {
                using namespace std::string_literals;
                logger.debug("AUTO: Registering factory: "s + typeid(FactoryType).name());
                factoryContext.template addFactory<FactoryType>();
            });
        }
    }// namespace detail

    //@todo add concept FactoryType is derived from Factory template
    template<typename FactoryType,
             typename ProducedObjectType,
             typename FactorySpecialization = NoFactorySpecialization,
             typename Traits                = FactoryTraits<ProducedObjectType, FactorySpecialization>>
    class AutoFactory : public Factory<ProducedObjectType, FactorySpecialization, Traits>,
                        protected AutoCaller<FactoryType, detail::registerFactoryInFactoryContext<FactoryType>> {
    public:
        AutoFactory() : AutoCaller<FactoryType, detail::registerFactoryInFactoryContext<FactoryType>>({}){};
    };


    template<template<typename, typename> class Factory ,typename ProducedObjectType, typename FactorySpecialization>
    using TemplateAutoFactory = AutoFactory<Factory<ProducedObjectType, FactorySpecialization>, ProducedObjectType, FactorySpecialization>;

}// namespace bsc

#endif