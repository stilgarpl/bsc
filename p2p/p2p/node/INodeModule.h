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
#include <any>

class INodeModule : public virtual IDependencyManaged, public Runnable {
private:
    Uber<Type> submodule;
protected:
    INode &node;
    std::shared_ptr<void> _config;

    template<typename ModuleType>
    typename ModuleType::Configuration &getConfiguration() {
        if (_config == nullptr) {
            _config = std::make_shared<typename ModuleType::Configuration>();
        }
        return *std::static_pointer_cast<typename ModuleType::Configuration>(_config);
    }

public:
    using Configuration = IConfig;
    using ModuleIdType = std::string;


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

    void doSaveConfiguration();

    virtual void initialize()=0;

    virtual void ready() = 0;

    virtual void shutdown() = 0;

    virtual void prepareSubmodules() {};
    //virtual IConfig &configuration()=0;

    virtual bool setupLogic()=0;
    virtual void initializeConfiguration()=0;
    virtual void saveConfiguration() = 0;
    virtual void loadConfiguration() = 0;

    virtual const ModuleIdType &getModuleId() const = 0;

protected:
    //module events
    //@todo maybe integrate somehow changeModuleState with changeState? the problem is, changeModuleState generated event with real module type. changeState just does this type
    virtual void changeModuleState(const ModuleState &state) = 0;

};



//typedef std::shared_ptr<INodeModule> INodeModulePtr;



#endif //BASYCO_INODEMODULE_H
