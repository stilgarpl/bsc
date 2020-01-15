//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_UBER_H
#define BASYCO_UBER_H

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

        //BaseUber() : containers(1) {}

    };

/**
 * Uber implementation where containers are templates, with real parameters passed in get<>()
 * @tparam Container
 */
    template<template<typename...> typename Container>
    class Uber : public bsc::BaseUber {
    public:

        template<typename... Args, typename ... ConstructorArgs>
        Container<Args...>& get(ConstructorArgs ... constructorArgs) {
            std::unique_lock<std::mutex> lock(containerLock);
            typedef Container<Args...> ContainerType;
            const static auto typeId = getTypeId<Args...>();
            auto& ref = containers[typeId];
            if (ref == nullptr) {
                ref = std::make_shared<ContainerType>(constructorArgs...);

            }
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


#endif //BASYCO_UBER_H
