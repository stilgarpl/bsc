//
// Created by stilgar on 05.11.17.
//

#ifndef BASYCO_INODE_H
#define BASYCO_INODE_H


#include <p2p/dependency/DependencyManager.h>
#include "INodeModule.h"

class INode {
protected:
    std::mutex lock;
public:
    //is this necessary?
    virtual LogicManager &getLogicManager() =0;

    virtual void setNodeContextActive() =0;

protected:
    virtual INodeModulePtr getModuleByDependencyId(DependencyManager::TypeIdType id) = 0;

public:

    template<typename ModuleType>
    ModuleTypePtr<ModuleType> getModule() {
        //  std::lock_guard<std::mutex> g(lock);
        auto id = DependencyManager::getClassId<ModuleType>();
        //possible to cast, we know exactly what type this pointer has (it has either good type or it's null
        return std::static_pointer_cast<ModuleType>(getModuleByDependencyId(id));
    }

};

#endif //BASYCO_INODE_H
