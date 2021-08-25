//
// Created by Krzysztof Tulidowicz on 23.08.17.
//

#ifndef BSC_UBER_H
#define BSC_UBER_H

#include <map>
#include <memory>
#include <mutex>
#include <functional>
#include <atomic>

namespace bsc {
    class BaseUber {
    private:
        mutable std::mutex idLock;
    protected:
        mutable std::mutex containerLock;

    protected:
        typedef unsigned int TypeIdType;
        std::map<TypeIdType, std::shared_ptr<void>> containers;
//@todo replace getNextTypeId with type id Generator
        TypeIdType getNextTypeId() const {
            static std::atomic<TypeIdType> val = 0;
            return val++;
        }

        template<typename...>
        TypeIdType getTypeId() const {
            std::unique_lock<std::mutex> lock(idLock);
            const static auto typeId = getNextTypeId();
            return typeId;
        }

        template<auto >
        TypeIdType getTypeId() const {
            std::unique_lock<std::mutex> lock(idLock);
            const static auto typeId = getNextTypeId();
            return typeId;
        }

        //BaseUber() : containers(1) {}

    };

/**
 * Uber implementation where containers are templates, with real parameters passed in get<>()
 * @tparam Container
 */
    template<template<typename ...> typename Container>
    class Uber : public bsc::BaseUber {
    public:

        template<typename... Args, typename ... ConstructorArgs>
        auto& get(ConstructorArgs ... constructorArgs) {
            std::unique_lock<std::mutex> lock(containerLock);
            typedef Container<Args...> ContainerType;
            const static auto typeId = getTypeId<Args...>();
            auto& ref = containers[typeId];
            if (ref == nullptr) {
                ref = std::make_shared<ContainerType>(constructorArgs...);

            }
            auto& result = *std::static_pointer_cast<ContainerType>(ref);
            return result;

        }

        /**
         * this method will only work if all containers have the same base class
         * @tparam BaseClass
         * @param func
         */
        template<typename BaseClass>
        void forEach(std::function<void(BaseClass&)> func) {
            std::unique_lock<std::mutex> lock(containerLock);
            for (auto &&[key, it] : containers) {
                if (it != nullptr) {
                    BaseClass& result = *std::static_pointer_cast<BaseClass>(it);
                    func(result);
                }
            }
        }
    };

    /**
 * Uber implementation where containers are templates, with real parameters passed in get<>()
 * @tparam Container
 */
 //@todo template<auto...> -> Args... add variadic if possible
    template<template<auto> typename Container>
    class NonTypeUber : public bsc::BaseUber {
    public:

        template<auto Args, typename ... ConstructorArgs>
        Container<Args>& get(ConstructorArgs ... constructorArgs) {
            std::unique_lock<std::mutex> lock(containerLock);
            typedef Container<Args> ContainerType;
            const static auto typeId = getTypeId<Args>();
            auto& ref = containers[typeId];
            if (ref == nullptr) {
                ref = std::make_shared<ContainerType>(constructorArgs...);

            }
            ContainerType& result = *std::static_pointer_cast<ContainerType>(ref);
            return result;

        }

        template<auto Args>
        bool has() const {
            std::unique_lock<std::mutex> lock(containerLock);
            const static auto typeId = getTypeId<Args>();
            return containers.contains(typeId);
        }


        template<auto Args, typename ... ConstructorArgs>
        const Container<Args>& get(ConstructorArgs ... constructorArgs) const {
            std::unique_lock<std::mutex> lock(containerLock);
            typedef Container<Args> ContainerType;
            const static auto typeId = getTypeId<Args>();
            const auto& ref = containers.at(typeId);
            ContainerType& result = *std::static_pointer_cast<ContainerType>(ref);
            return result;

        }

        /**
         * this method will only work if all containers have the same base class
         * @tparam BaseClass
         * @param func
         */
        template<typename BaseClass>
        void forEach(std::function<void(BaseClass&)> func) {
            std::unique_lock<std::mutex> lock(containerLock);
            for (auto &&[key, it] : containers) {
                if (it != nullptr) {
                    BaseClass& result = *std::static_pointer_cast<BaseClass>(it);
                    func(result);
                }
            }
        }
    };

/**
 * Uber implementation where container is always the same, predefined type
 * @tparam Container
 */
    template<typename Container>
    class StaticUber : bsc::BaseUber {
    public:

        template<typename... Args>
        Container& get() {
            std::unique_lock<std::mutex> lock(containerLock);
            typedef Container ContainerType;
            const static auto typeId = getTypeId<Args...>();
//        if (containers.size() <= typeId) {
//            containers.resize(10*typeId + 20);
//        }
            auto& ref = containers[typeId];
            if (ref == nullptr) {
                ref = std::make_shared<ContainerType>();

            }
            ContainerType& result = *std::static_pointer_cast<ContainerType>(ref);
            return result;

        }

        void forEach(std::function<void(Container&)> func) {
            std::unique_lock<std::mutex> lock(containerLock);
            for (auto &&[key, it] : containers) {
                if (it != nullptr) {
                    Container& result = *std::static_pointer_cast<Container>(it);
                    func(result);
                }
            }
        }
    };
}


template<typename T, typename ... TypeArgs>
class Type {
private:
    T t;
public:
    operator T&() {
        return t;
    }

    Type& operator=(const T& other) {
        t = other;
        return *this;
    }

    T& getType() {
        return t;
    }
};


#endif //BSC_UBER_H
