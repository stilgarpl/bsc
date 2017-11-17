//
// Created by stilgar on 17.11.17.
//

#ifndef BASYCO_INODEMODULE_H
#define BASYCO_INODEMODULE_H


#include <p2p/thread/Runnable.h>
#include <p2p/dependency/IDependencyManaged.h>
#include <p2p/logic/ILogicModule.h>


class INodeModule : public ILogicModule, public virtual IDependencyManaged, public Runnable {

public:
    virtual void initialize()=0;

};

//typedef std::shared_ptr<INodeModule> INodeModulePtr;

template<typename ModuleType>
using ModuleTypePtr = std::shared_ptr<ModuleType>;

typedef ModuleTypePtr<INodeModule> INodeModulePtr;

#endif //BASYCO_INODEMODULE_H
