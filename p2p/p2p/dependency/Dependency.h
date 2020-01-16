//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_DEPENDENCY_H
#define BASYCO_DEPENDENCY_H


#include <vector>
#include <array>
#include "IDependency.h"
#include "DependencyManager.h"

namespace bsc {
    template<typename T, typename ... Args>
    class Dependency : public IDependency {

    public:
        // typedef std::vector<typename DependencyManager::TypeIdType/*, 1+sizeof... (Args)*/> ArrayType;
        typedef DependencyManager::ArrayType ArrayType;
    private:
        // ArrayType dependency;
    public:
        //@todo constexpr?
        static ArrayType getDependencyIds() {
            ArrayType ret;
            ret.reserve(1 + sizeof...(Args));
            ret.push_back(DependencyManager::getClassId<T>());
            auto rest = Dependency<Args...>::getDependencyIds();
            ret.insert(ret.end(), rest.begin(), rest.end());
            return ret;
        }

    public:
        ArrayType getDependencyIdents() override {
            return getDependencyIds();
        }

        ~Dependency() override = default;
    };

    template<typename T>
    class Dependency<T> : public IDependency {

    public:
        typedef std::vector<typename DependencyManager::TypeIdType/*, 1+sizeof... (Args)*/> ArrayType;

        //@todo constexpr?
        static ArrayType getDependencyIds() {
            ArrayType ret;
            ret.push_back(DependencyManager::getClassId<T>());
            return ret;
        }

        ArrayType getDependencyIdents() override {
            return getDependencyIds();
        }
    };

    class DependencyPack : public IDependency {
        typedef DependencyManager::ArrayType ArrayType;
    private:
        ArrayType dependency;


    public:
        template<typename... Arrays>
        void addDependency(const ArrayType& a, Arrays... arrays) {
            addDependency(a);
            addDependency(arrays...);

        }

        void addDependency(const ArrayType& a) {
            dependency.insert(dependency.end(), a.begin(), a.end());
        }

        DependencyPack(const ArrayType& dependency) : dependency(dependency) {

        }

        DependencyPack() = default;

    public:
        ArrayType getDependencyIdents() override {
            return dependency;
        }

    };

    //@todo how to empty template?
    class Dependency1 : public IDependency {

    public:
        typedef std::vector<typename DependencyManager::TypeIdType/*, 1+sizeof... (Args)*/> ArrayType;

        static ArrayType getDependencyIds() {
            const ArrayType ret;
            //ret.push_back(DependencyManager::getClassId<T>());
            return ret;
        }

        ArrayType getDependencyIdents() override {
            return getDependencyIds();
        }
    };

}


#endif //BASYCO_DEPENDENCY_H
