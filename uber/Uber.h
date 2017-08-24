//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_UBER_H
#define BASYCO_UBER_H

#include <vector>
#include <memory>

class BaseUber {
protected:
    typedef unsigned int TypeIdType;
    std::vector<std::shared_ptr<void>> containers;

    const TypeIdType getNextTypeId() const {
        static TypeIdType val = 0;
        return val++;
    }

    template<typename...>
    const TypeIdType getTypeId() const {
        const static auto typeId = getNextTypeId();
        return typeId;
    };

    //BaseUber() : containers(1) {}

};


template<template<typename...> typename container>
class Uber : public BaseUber {
public:

    template<typename... Args>
    container<Args...> &get() {
        typedef container<Args...> ContainerType;
        const static auto typeId = getTypeId<Args...>();
        if (containers.size() <= typeId) {
            containers.resize(typeId + 2);
        }
        auto &ref = containers[typeId];
        if (ref == nullptr) {
            ref = std::make_shared<ContainerType>();

        }
        ContainerType &result = *std::static_pointer_cast<ContainerType>(ref);
        return result;

    }
};

template<typename container>
class StaticUber : BaseUber {
public:

    template<typename... Args>
    container &get() {
        typedef container ContainerType;
        const static auto typeId = getTypeId<Args...>();
        if (containers.size() <= typeId) {
            containers.resize(typeId + 2);
        }
        auto &ref = containers[typeId];
        if (ref == nullptr) {
            ref = std::make_shared<ContainerType>();

        }
        ContainerType &result = *std::static_pointer_cast<ContainerType>(ref);
        return result;

    }
};


template<typename T>
class type {
private:
    T t;
public:
    operator T &() {
        return t;
    }
};


#endif //BASYCO_UBER_H
