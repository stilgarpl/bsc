//
// Created by Krzysztof Tulidowicz on 15.10.2020.
//

#ifndef BSC_FACTORYPTR_H
#define BSC_FACTORYPTR_H

#include "Factory.h"
#include "FactoryContext.h"
#include <context/context/Context.h>

namespace bsc {
    template<typename ProducedObjectType,
             typename FactorySpecialization = NoFactorySpecialization,
             typename Traits                = FactoryTraits<ProducedObjectType, FactorySpecialization>>
    class FactoryPtr {
    public:
        using FactoryType  = Factory<ProducedObjectType, FactorySpecialization, Traits>;
    private:
        using Ptr      = typename FactoryType::Ptr;

        Ptr factoryPtr = nullptr;

    public:
        //@todo maybe this should be in a separate type? FactoryAutoPtr or sth? it's this surprising that default constructed ptr has value
        //and it's not empty? Or rename this whole class SharedFactoryPtr ? I think it would be best if I move this one constructor to
        //DERIVED class
        FactoryPtr() {
            factoryPtr = Context::getActiveContext()
                                 ->get<FactoryContext>()
                                 ->template getFactory<ProducedObjectType, FactorySpecialization, Traits>();
        }
        FactoryPtr(const Ptr& ptr) : factoryPtr(ptr){};

        Ptr operator->() const{
            if (factoryPtr== nullptr) {
                throw std::invalid_argument("Factory pointer is null");
            }
            return factoryPtr;
        }

        template<typename FactoryType>
        auto as() {
            return FactoryType::template factoryPointerCast<FactoryType>(factoryPtr);
        }

        bool isValid() const {
            return factoryPtr != nullptr;
        }
    };
}// namespace bsc

#endif