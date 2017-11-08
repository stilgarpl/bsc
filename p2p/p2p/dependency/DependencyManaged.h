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

    void setRequired(const std::shared_ptr<IDependency> &required) {
        DependencyManaged::required = required;
    }

    virtual IDependency::TypeIdType getDepedencyId() {
        return DependencyManager::getClassId<T>();
    }

protected:

    template<typename ... Args>
    void setRequired() {
        required = std::make_shared<Dependency<Args...>>();
    }
};


#endif //BASYCO_DEPENDECYMANAGED_H
