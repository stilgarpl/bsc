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
private:
    Uber<Type> submodule;
protected:
    INode &node;

public:
    typedef IConfig Config;


    class SubModule {
    };

    template<typename OtherModule>
    typename OtherModule::SubModule &getSubModule() {
        return submodule.get<typename OtherModule::SubModule>().getType();
    }

    explicit INodeModule(INode &node) : node(node) {}

    void doInitialize() {
        initialize();
        changeModuleState(ModuleState::INITIALIZED);
    }

    void doSetupLogic() {
        if (setupLogic()) {
            changeModuleState(ModuleState::LOGIC_READY);
        }

    }

    void doReady() {
        ready();
        changeModuleState(ModuleState::READY);
    }

    void doShutdown() {
        shutdown();
        changeModuleState(ModuleState::SHUTDOWN);
    }
    virtual void initialize()=0;

    virtual void ready() = 0;

    virtual void shutdown() = 0;
    virtual IConfig &configuration()=0;

    virtual bool setupLogic()=0;
    virtual void initializeConfiguration()=0;

protected:
    //module events
    //@todo maybe integrate somehow changeModuleState with changeState? the problem is, changeModuleState generated event with real module type. changeState just does this type
    virtual void changeModuleState(const ModuleState &state) = 0;

};

//typedef std::shared_ptr<INodeModule> INodeModulePtr;



#endif //BASYCO_INODEMODULE_H
