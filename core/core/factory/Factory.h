//
// Created by stilgar on 30.08.2019.
//

#ifndef BSC_FACTORY_H
#define BSC_FACTORY_H


#include <memory>
#include <string>

namespace bsc {

    class NoFactorySpecialization {};
    //    using NoFactorySpecialization = void;

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

    template<bool hasArgs,
             typename producedObjectType,
             typename factorySpecialization = NoFactorySpecialization,
             typename Traits                = FactoryTraits<producedObjectType, factorySpecialization>>
    struct BaseFactory {};

    template<typename producedObjectType, typename factorySpecialization, typename Traits>
    struct BaseFactory<false, producedObjectType, factorySpecialization, Traits> {
        using ProducedObjectType    = producedObjectType;
        using FactorySpecialization = factorySpecialization;
        using SelectorType          = typename Traits::SelectorType;

        virtual ProducedObjectType create(const SelectorType& selector) const = 0;
    };

    template<typename producedObjectType, typename factorySpecialization, typename Traits>
    struct BaseFactory<true, producedObjectType, factorySpecialization, Traits> {
        using ProducedObjectType    = producedObjectType;
        using FactorySpecialization = factorySpecialization;
        using SelectorType          = typename Traits::SelectorType;
        using ArgumentType          = typename Traits::ArgumentType;

        //@todo it would be awesome if ArgumentType could be a parameter pack... investigate if this is possible
        virtual ProducedObjectType create(const SelectorType& selector, const ArgumentType& argument) const = 0;
        //@todo let's assume that ArgumentType is default constructible... if not, add a way to not have this overload
        // for that case
        ProducedObjectType create(const SelectorType& selector) const { return create(selector, {}); }
    };

    class FactoryInvalidSelector : public std::domain_error {
    public:
        FactoryInvalidSelector(const std::string& arg) : domain_error(arg) {}
    };

    template<typename ProducedObjectType,
             typename FactorySpecialization = NoFactorySpecialization,
             typename Traits                = FactoryTraits<ProducedObjectType, FactorySpecialization>>
    struct Factory
        : public BaseFactory<HasArgumentType<Traits>::value, ProducedObjectType, FactorySpecialization, Traits> {};

    template<typename ProducedObjectType, typename FactorySpecialization = NoFactorySpecialization>
    using FactoryPtr = std::shared_ptr<Factory<ProducedObjectType, FactorySpecialization>>;

}// namespace bsc

#endif//BSC_FACTORY_H
