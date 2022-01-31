//
// Created by Krzysztof Tulidowicz on 30.08.2019.
//

#ifndef BSC_FACTORY_H
#define BSC_FACTORY_H

#include <memory>
#include <set>
#include <string>

namespace bsc {

    class NoFactorySpecialization {};
    //    using NoFactorySpecialization = void;

    template<typename ProducedObjectType, typename FactorySpecialization = NoFactorySpecialization>
    struct FactoryTraits {
        using SelectorType = std::string;
        // using ArgumentType = ...
    };

    template<typename T>
    concept HasArgumentType = requires(T t) {
        typename T::ArgumentType;
    };

    template<bool hasArgs,
             typename producedObjectType,
             typename factorySpecialization = NoFactorySpecialization,
             typename Traits                = FactoryTraits<producedObjectType, factorySpecialization>>
    struct BaseFactory {};

    template<typename producedObjectType, typename factorySpecialization, typename traits>
    struct BaseFactory<false, producedObjectType, factorySpecialization, traits> {
        using ProducedObjectType                                              = producedObjectType;
        using FactorySpecialization                                           = factorySpecialization;
        using Traits                                                          = traits;
        using SelectorType                                                    = typename Traits::SelectorType;

        virtual ProducedObjectType create(const SelectorType& selector) const = 0;
    };

    template<typename producedObjectType, typename factorySpecialization, typename traits>
    struct BaseFactory<true, producedObjectType, factorySpecialization, traits> {
        using ProducedObjectType                                                                            = producedObjectType;
        using FactorySpecialization                                                                         = factorySpecialization;
        using Traits                                                                                        = traits;
        using SelectorType                                                                                  = typename Traits::SelectorType;
        using ArgumentType                                                                                  = typename Traits::ArgumentType;

        //@todo it would be awesome if ArgumentType could be a parameter pack... investigate if this is possible
        virtual ProducedObjectType create(const SelectorType& selector, const ArgumentType& argument) const = 0;
        //@todo C++20 let's assume that ArgumentType is default constructible... if not, add a way to not have this overload
        // for that case, probably using concepts
        ProducedObjectType create(const SelectorType& selector) const {
            return create(selector, {});
        }
    };

    class FactoryInvalidSelector : public std::domain_error {
    public:
        explicit FactoryInvalidSelector(const std::string& arg) : domain_error(arg) {
        }
    };

    class FactoryInterfaceError : public std::domain_error {
    public:
        FactoryInterfaceError(const std::string& arg) : domain_error(arg) {
        }
    };

    template<typename ProducedObjectType,
             typename FactorySpecialization = NoFactorySpecialization,
             typename Traits                = FactoryTraits<ProducedObjectType, FactorySpecialization>>
    struct Factory : public BaseFactory<HasArgumentType<Traits>, ProducedObjectType, FactorySpecialization, Traits> {

        static constexpr bool hasArgumentType = HasArgumentType<Traits>;

        using Ptr                             = std::shared_ptr<Factory<ProducedObjectType, FactorySpecialization, Traits>>;
        virtual std::set<typename Traits::SelectorType> getSelectors() const {
            throw FactoryInterfaceError("getSelectors not implemented for this factory");
        }

    private:
        template<typename T>
        static auto factoryPointerCast(const Ptr& ptr) {
            auto result = std::dynamic_pointer_cast<T>(ptr);
            if (result == nullptr) {
                throw std::invalid_argument("Invalid pointer cast");
            }
            return result;
        }

        template<typename, typename, typename>
        friend class FactoryPtr;
    };

}// namespace bsc

#endif// BSC_FACTORY_H
