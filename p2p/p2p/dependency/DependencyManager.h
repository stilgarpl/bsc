//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_DEPENDENCYMANAGER_H
#define BASYCO_DEPENDENCYMANAGER_H

#include <vector>
#include <memory>

#include <list>
#include <algorithm>
#include <p2p/log/Logger.h>
#include <iostream>
#include "IDependency.h"
#include "IDependencyManaged.h"


class DependencyManager {
public:
    typedef IDependency::TypeIdType TypeIdType;
    typedef IDependency::ArrayType ArrayType;
protected:

    static const TypeIdType getNextTypeId() {
        static TypeIdType val = 0;
        return val++;
    }

    template<typename...>
    static const TypeIdType getTypeId() {
        const static auto typeId = getNextTypeId();
        return typeId;
    };

public:

    template<typename T>
    static TypeIdType getClassId(T *t = nullptr) {
        return getTypeId<T>();
    }

    typedef std::list<std::shared_ptr<IDependencyManaged>> DependencyList;

    static DependencyList dependencySort(DependencyList source);

};


#endif //BASYCO_DEPENDENCYMANAGER_H
