//
// Created by stilgar on 30.08.2019.
//

#ifndef BSC_FACTORY_H
#define BSC_FACTORY_H


#include <memory>
#include <string>

namespace bsc {

    class NoFactorySpecialization {};

    template<typename ProducedObjectType, typename FactorySpecialization = NoFactorySpecialization>
    struct FactoryTraits {
        using SelectorType = std::string;
        //using ArgumentType = ...
    };

    template<typename T>
    class HasArgumentType {
        template<typename C>
        constexpr static bool f(typename C::ArgumentType*) {
            return true;
        }

        template<typename C>
        constexpr static bool f(...) {
            return false;
        }

    public:
        static const bool value = f<T>(0);
    };

    template<bool hasArgs, typename ProducedObjectType, typename FactorySpecialization = NoFactorySpecialization>
    struct BaseFactory {
    };

    template<typename producedObjectType, typename factorySpecialization>
    struct BaseFactory<false, producedObjectType, factorySpecialization> {
        using ProducedObjectType = producedObjectType;
        using FactorySpecialization = factorySpecialization;
        using SelectorType = typename FactoryTraits<ProducedObjectType, FactorySpecialization>::SelectorType;

        virtual ProducedObjectType create(const SelectorType& selector) = 0;
    };

    template<typename producedObjectType, typename factorySpecialization>
    struct BaseFactory<true, producedObjectType, factorySpecialization> {
        using ProducedObjectType = producedObjectType;
        using FactorySpecialization = factorySpecialization;
        using SelectorType = typename FactoryTraits<ProducedObjectType, FactorySpecialization>::SelectorType;
        using ArgumentType = typename FactoryTraits<ProducedObjectType, FactorySpecialization>::ArgumentType;

        //@todo it would be awesome if ArgumentType could be a parameter pack... investigate if this is possible
        virtual ProducedObjectType create(const SelectorType& selector, const ArgumentType& argument) = 0;
        //@todo let's assume that ArgumentType is defualt constructible... if not, add a way to not have this overload for that case
        ProducedObjectType create(const SelectorType& selector) {
            return create(selector,{});
        }
    };


    template<typename ProducedObjectType, typename FactorySpecialization = NoFactorySpecialization>
    struct Factory
        : public BaseFactory<HasArgumentType<FactoryTraits<ProducedObjectType, FactorySpecialization>>::value, ProducedObjectType, FactorySpecialization> {
    };

    template<typename ProducedObjectType, typename FactorySpecialization=NoFactorySpecialization>
    using FactoryPtr = std::shared_ptr<Factory<ProducedObjectType, FactorySpecialization>>;

}// namespace bsc

#endif//BSC_FACTORY_H
