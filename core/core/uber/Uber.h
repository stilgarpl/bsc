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
 * @tparam container
 */
template<template<typename...> typename container>
class Uber : public BaseUber {
public:

    template<typename... Args, typename ... ConstructorArgs>
    container<Args...> &get(ConstructorArgs ... constructorArgs) {
        std::unique_lock<std::mutex> lock(containerLock);
        typedef container<Args...> ContainerType;
        const static auto typeId = getTypeId<Args...>();
        auto &ref = containers[typeId];
        if (ref == nullptr) {
            ref = std::make_shared<ContainerType>(constructorArgs...);

        }
        ContainerType &result = *std::static_pointer_cast<ContainerType>(ref);
        return result;

    }

    /**
     * this method will only work if all containers have the same base class
     * @tparam BaseClass
     * @param func
     */
    template<typename BaseClass>
    void forEach(std::function<void(BaseClass &)> func) {
        std::unique_lock<std::mutex> lock(containerLock);
        for (auto &&[key, it] : containers) {
            if (it != nullptr) {
                BaseClass &result = *std::static_pointer_cast<BaseClass>(it);
                func(result);
            }
        }
    }
};

/**
 * Uber implementation where container is always the same, predefined type
 * @tparam container
 */
template<typename container>
class StaticUber : BaseUber {
public:

    template<typename... Args>
    container &get() {
        std::unique_lock<std::mutex> lock(containerLock);
        typedef container ContainerType;
        const static auto typeId = getTypeId<Args...>();
//        if (containers.size() <= typeId) {
//            containers.resize(10*typeId + 20);
//        }
        auto &ref = containers[typeId];
        if (ref == nullptr) {
            ref = std::make_shared<ContainerType>();

        }
        ContainerType &result = *std::static_pointer_cast<ContainerType>(ref);
        return result;

    }

    void forEach(std::function<void(container &)> func) {
        std::unique_lock<std::mutex> lock(containerLock);
        for (auto &&[key, it] : containers) {
            if (it != nullptr) {
                container &result = *std::static_pointer_cast<container>(it);
                func(result);
            }
        }
    }
};


template<typename T, typename ... TypeArgs>
class Type {
private:
    T t;
public:
    operator T &() {
        return t;
    }

    Type &operator=(const T &other) {
        t = other;
        return *this;
    }

    T &getType() {
        return t;
    }
};


#endif //BASYCO_UBER_H
