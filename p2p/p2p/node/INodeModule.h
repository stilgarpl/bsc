//
// Created by stilgar on 17.11.17.
//

#ifndef BASYCO_INODEMODULE_H
#define BASYCO_INODEMODULE_H


#include <p2p/thread/Runnable.h>
#include <p2p/dependency/IDependencyManaged.h>
//@todo this is interface for configuration, it shouldn't have dependencies on modules
#include <p2p/modules/configuration/IConfig.h>
#include "INode.h"
#include "ModuleState.h"

class INodeModule : public virtual IDependencyManaged, public Runnable {
private:
    Uber<Type> submodule;
protected:
    INode &node;
    std::unique_ptr<IConfig> _config;

    template<typename ModuleType>
    typename ModuleType::Configuration &getConfiguration() {
        if (_config == nullptr) {
            _config = std::make_unique<typename ModuleType::Configuration>();
        }
        return static_cast<typename ModuleType::Configuration &>(*_config);
    }

public:
    typedef IConfig Config;
    using Configuration = IConfig;


    class SubModule {
    };

    template<typename OtherModule>
    typename OtherModule::SubModule &getSubModule() {
        return submodule.get<typename OtherModule::SubModule>().getType();
    }

    explicit INodeModule(INode &node) : node(node) {}

    void doInitialize();

    void doSetupLogic();

    void doReady();

    void doShutdown();

    void doPrepareSubmodules();

    virtual void initialize()=0;

    virtual void ready() = 0;

    virtual void shutdown() = 0;

    virtual void prepareSubmodules() {};
    //virtual IConfig &configuration()=0;

    virtual bool setupLogic()=0;
    virtual void initializeConfiguration()=0;

protected:
    //module events
    //@todo maybe integrate somehow changeModuleState with changeState? the problem is, changeModuleState generated event with real module type. changeState just does this type
    virtual void changeModuleState(const ModuleState &state) = 0;

};



//typedef std::shared_ptr<INodeModule> INodeModulePtr;



#endif //BASYCO_INODEMODULE_H
