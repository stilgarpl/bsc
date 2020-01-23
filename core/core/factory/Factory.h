//
// Created by stilgar on 30.08.2019.
//

#ifndef BSC_FACTORY_H
#define BSC_FACTORY_H


#include <memory>
#include <string>

namespace bsc {
    template<typename ProducedObjectType, typename ...  FactorySpecialization>
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

    template<bool hasArgs, typename ProducedObjectType, typename ...  FactorySpecialization>
    struct BaseFactory {

    };

    template<typename ProducedObjectType, typename ...  FactorySpecialization>
    struct BaseFactory<false, ProducedObjectType, FactorySpecialization...> {
        using SelectorType = typename FactoryTraits<ProducedObjectType, FactorySpecialization...>::SelectorType;

        virtual ProducedObjectType create(const SelectorType& selector) = 0;
    };

    template<typename ProducedObjectType, typename ...  FactorySpecialization>
    struct BaseFactory<true, ProducedObjectType, FactorySpecialization...> {
        using SelectorType = typename FactoryTraits<ProducedObjectType, FactorySpecialization...>::SelectorType;
        using ArgumentType = typename FactoryTraits<ProducedObjectType, FactorySpecialization...>::ArgumentType;

        virtual ProducedObjectType create(const SelectorType& selector, const ArgumentType& argument) = 0;
    };


    template<typename ProducedObjectType, typename ...  FactorySpecialization>
    struct Factory
            : public BaseFactory<HasArgumentType<FactoryTraits<ProducedObjectType, FactorySpecialization...>>::value, ProducedObjectType, FactorySpecialization...> {
    };

    template<typename ProducedObjectType, typename ...  FactorySpecialization>
    using FactoryPtr = std::shared_ptr<Factory<ProducedObjectType, FactorySpecialization...>>;

}

#endif //BSC_FACTORY_H
