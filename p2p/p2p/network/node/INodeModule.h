//
// Created by stilgar on 17.11.17.
//

#ifndef BASYCO_INODEMODULE_H
#define BASYCO_INODEMODULE_H


#include <p2p/thread/Runnable.h>
#include <p2p/dependency/IDependencyManaged.h>
#include <p2p/logic/ILogicModule.h>
///@todo this is interface for configuration, it shouldn't have dependencies on modules
#include <p2p/modules/configuration/IConfig.h>

class INodeModule : public ILogicModule, public virtual IDependencyManaged, public Runnable {

public:
    typedef IConfig Config;
    virtual void initialize()=0;

    virtual IConfig &configuration()=0;

    virtual void initializeConfiguration()=0;

};

//typedef std::shared_ptr<INodeModule> INodeModulePtr;

template<typename ModuleType>
using ModuleTypePtr = std::shared_ptr<ModuleType>;

typedef ModuleTypePtr<INodeModule> INodeModulePtr;

#endif //BASYCO_INODEMODULE_H
