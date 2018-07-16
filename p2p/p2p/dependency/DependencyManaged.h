//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_DEPENDECYMANAGED_H
#define BASYCO_DEPENDECYMANAGED_H

//
//#include "DependencyManager.h"
#include "IDependency.h"
#include "IDependencyManaged.h"
#include "DependencyManager.h"
#include "Dependency.h"

#include <memory>


template<typename T>
class DependencyManaged : public virtual IDependencyManaged {
private:
    std::shared_ptr<IDependency> required;
public:

    virtual const std::shared_ptr<IDependency> &getRequiredDependencies() const {
        return required;
    }

    virtual IDependency::TypeIdType getDependencyId() {
        return DependencyManager::getClassId<T>();
    }

    void setRequired(const std::shared_ptr<IDependency> &required) {
        DependencyManaged::required = required;


        //clean required - remove duplicates and references to itself
        //@todo duplicate clean
        //remove self references
        auto &&ids = required->getDependencyIdents();

        if (std::find_if(ids.begin(), ids.end(), [&, this](auto i) -> bool { return i == this->getDependencyId(); }) !=
            ids.end()) {
            ids.erase(std::remove_if(ids.begin(), ids.end(),
                                     [&, this](auto i) -> bool { return i == this->getDependencyId(); }));
            DependencyManaged::required = std::make_shared<DependencyPack>(ids);
        }
    }


protected:

    template<typename ... Args>
    void setRequired() {
        //@todo remove duplicates
        if constexpr (sizeof... (Args) >= 1) {
            required = std::make_shared<Dependency<Args...>>();
        }
    }

protected:
    template<typename ... Args>
    void addRequiredDependency() {

        if (required == nullptr) {
            setRequired<Args...>();
            return;
        }

        auto ids = Dependency<Args...>::getDependencyIds();

        for (auto &&ident : required->getDependencyIdents()) {
            ids.erase(std::remove_if(ids.begin(), ids.end(), [&](auto i) -> bool { return (i == ident); }), ids.end());
        }

//        LOGGER("before merge");
//        for (auto &&r : required->getDependencyIdents()) {
//            LOGGER("r =" + std::to_string(r));
//        }

        if (ids.size() > 0) {
            auto depPack = std::make_shared<DependencyPack>();
            depPack->addDependency(required->getDependencyIdents(), ids);
            //clean ids - remove duplicates and references to itself
            //@todo duplicate clean

            setRequired(depPack);
        } else {
//            LOGGER("NO DEPS")
            //nothing changes.
        }

//        LOGGER("after merge");
//        for (auto &&r : required->getDependencyIdents()) {
//            LOGGER("r =" +std::to_string(r));
//        }

    }
};


#endif //BASYCO_DEPENDECYMANAGED_H
