//
// Created by stilgar on 17.11.17.
//

#ifndef BASYCO_INODEMODULE_H
#define BASYCO_INODEMODULE_H


#include <p2p/thread/Runnable.h>
#include <p2p/dependency/IDependencyManaged.h>
#include <p2p/modules/logic/ILogicModule.h>
//@todo this is interface for configuration, it shouldn't have dependencies on modules
#include <p2p/modules/configuration/IConfig.h>
#include "INode.h"
#include "ModuleState.h"

class INodeModule : public virtual IDependencyManaged, public Runnable {
protected:
    INode &node;

public:
    typedef IConfig Config;

    explicit INodeModule(INode &node) : node(node) {}

    void doInitialize() {
        initialize();
        changeState(ModuleState::INITIALIZED);
    }

    void doSetupLogic() {
        if (setupLogic()) {
            changeState(ModuleState::LOGIC_READY);
        }

    }

    void doReady() {
        ready();
        changeState(ModuleState::READY);
    }

    void doShutdown() {
        shutdown();
        changeState(ModuleState::SHUTDOWN);
    }
    virtual void initialize()=0;

    virtual void ready() = 0;

    virtual void shutdown() = 0;
    virtual IConfig &configuration()=0;

    virtual bool setupLogic()=0;
    virtual void initializeConfiguration()=0;

protected:
    //module events
    virtual void changeState(const ModuleState &state) = 0;

};

//typedef std::shared_ptr<INodeModule> INodeModulePtr;



#endif //BASYCO_INODEMODULE_H
