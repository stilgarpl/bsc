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

        //  LOGGER("Class id for type " + std::string(typeid(T).name()) + " is " + std::to_string(getTypeId<T>()));
        return getTypeId<T>();
    }

    typedef std::list<std::shared_ptr<IDependencyManaged>> DependencyList;

    template<typename DependencyList>
    static DependencyList dependencySort(DependencyList source) {
        DependencyList result;

        while (source.size() > 0) {
            //take first element
            auto &e = source.front();
//               LOGGER("e = " + std::to_string(e->getDependencyId()) + " " + (typeid(*e).name()));
            //get its dependency list
            //@todo check if null?
            IDependency::ArrayType eDeps;
            if (e->getRequiredDependencies() != nullptr) {
                eDeps = e->getRequiredDependencies()->getDependencyIdents();
            } else {

            }
//            for (auto &&item : eDeps) {
//                std::cout << "edep : " << item << " "<< typeid(*e).name()<< std::endl;
//            }
            //remove fullfilled dependencies from it
            eDeps.erase(
                    std::remove_if(eDeps.begin(), eDeps.end(), [&](IDependency::TypeIdType it) -> bool {
                        auto found = std::find_if(result.begin(), result.end(),
                                                  [&](std::shared_ptr<IDependencyManaged> jt) -> bool {
                                                      // LOGGER("jt " + std::to_string(jt->getDependencyId()) + "  it :" + std::to_string(it));
                                                      return jt->getDependencyId() == it;
                                                  });
                        return found != result.end();
                    }), eDeps.end());
//            for (auto &&item : eDeps) {
//                std::cout << "bedep : " << item << " "<< typeid(*e).name()<< std::endl;
//            }
            //if all dependencies are met, add it to result, remove from source
            if (eDeps.size() == 0) {
                result.push_back(e);
                source.pop_front();
            } else {
                source.push_back(e);
                source.pop_front();
            }
            //if not, element = first unmet dependency
            //loop
        }
        return result;
    }

};


#endif //BASYCO_DEPENDENCYMANAGER_H
