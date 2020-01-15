#include <utility>

//
// Created by stilgar on 1/7/19.
//

#ifndef BASYCO_LOGICBUILDER_H
#define BASYCO_LOGICBUILDER_H


#include <string>
#include <core/uber/Uber.h>
#include <map>


class LogicBuilder {

    class ITypeRecord {
        std::string name;
    public:
        const std::string &getName() const {
            return name;
        }

        void setName(const std::string &name) {
            ITypeRecord::name = name;
        }

        explicit ITypeRecord(std::string name) : name(std::move(name)) {}
    };

    template <typename Class>
    class TypeRecord : public ITypeRecord{
    public:
        explicit TypeRecord(std::string name) : ITypeRecord(std::move(name)) {}
    };
private:

    bsc::Uber<TypeRecord> classRegistry;
    std::map<std::string, std::unique_ptr<ITypeRecord>> typeMap;

public:

    template<typename Class>
    void registerClass(std::string name) {
        //@todo name passed to constructor. what if this method is run more than once? produce error? ignore? check if name is the same?
        classRegistry.get<Class>(name);
        typeMap[name] =std::make_unique<TypeRecord<Class>>(name);
    }

    template<template<typename Class1> typename Func>
    void runFunction(std::string name) {

    }
};


#endif //BASYCO_LOGICBUILDER_H
