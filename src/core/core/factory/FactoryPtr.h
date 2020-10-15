//
// Created by Krzysztof Tulidowicz on 15.10.2020.
//

#ifndef BSC_FACTORYPTR_H
#define BSC_FACTORYPTR_H

#include "Factory.h"
#include "FactoryContext.h"
#include <core/context/Context.h>
namespace bsc {
    template<typename ProducedObjectType,
             typename FactorySpecialization = NoFactorySpecialization,
             typename Traits                = FactoryTraits<ProducedObjectType, FactorySpecialization>>
    class FactoryPtr {
    private:
        using Ptr      = typename Factory<ProducedObjectType, FactorySpecialization, Traits>::Ptr;
        Ptr factoryPtr = nullptr;

    public:
        FactoryPtr() {
            factoryPtr = Context::getActiveContext()
                                 ->get<FactoryContext>()
                                 ->template getFactory<ProducedObjectType, FactorySpecialization, Traits>();
        }
        FactoryPtr(const Ptr& ptr) : factoryPtr(ptr){};

        Ptr operator->() {
            //@todo should I check if factoryPtr != null? throw NPE?
            return factoryPtr;
        }

        bool isValid() const { return factoryPtr != nullptr; }
    };
}// namespace bsc

#endif