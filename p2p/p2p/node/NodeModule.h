//
// Created by stilgar on 20.10.17.
//

#ifndef BASYCO_NODEMODULE_H
#define BASYCO_NODEMODULE_H


#include <p2p/modules/logic/ILogicModule.h>
#include <p2p/dependency/IDependencyManaged.h>
#include <p2p/dependency/DependencyManaged.h>
#include <p2p/modules/configuration/ConfigurationManager.h>
#include <p2p/logic/sources/ModuleSource.h>
#include "INode.h"
#include "INodeModule.h"

class NodeModule : public ILogicModule {

public:

    //@todo pure or not?
    void initialize() override {};

    void ready() override {};

    void shutdown() override {};
    explicit NodeModule(INode &node) : ILogicModule(node) {}

    void run() override {
        //do nothing, just so modules do not have to implement this if they don't want to
    }

protected:
    ConfigurationManager &getConfigurationManager() {
        return node.getConfigurationManager();
    }
};


template<typename T, typename ConfigType, typename ... Args>
class NodeModuleConfigDependent : public NodeModule, public DependencyManaged<T> {
private:
    typedef ConfigType Config;
    Config _config;
public:
//    typename T::Config _config;

    auto &getOwnSubModule() {
        return getSubModule<T>();
    }



private:
    template<typename T1, typename... Args1>
    void checkAndAddModules() {
        if (!node.hasModule<T1>()) {
            node.addModule<T1>();
            LOGGER(std::string("MODULE NOT FOUND, ADDING MODULE") + typeid(T1).name());
        }
        if constexpr (sizeof...(Args1) > 0) {
            checkAndAddModules<Args1...>();
        }
    }

public:
    explicit NodeModuleConfigDependent(INode &node) : NodeModule(node) {
        this->template setRequired<Args...>();
        if constexpr (sizeof...(Args) > 0) {
            checkAndAddModules<Args...>();
        }
    };

    Config &configuration() override {
        return _config;
    }

    void initializeConfiguration() override {
        auto loaded = node.getConfigurationManager().template load<Config>(configuration().getConfigId());
        LOGGER(std::string("Checking configuration for class: ") + typeid(T).name());
        if (loaded != nullptr) {
            configuration() = *loaded;
            LOGGER("Configuration found, loading")
        }
    }

protected:
    void changeModuleState(const ModuleState &state) override {
        ILogicModule::changeModuleState(state);
        node.getLogicManager().template requireSource<ModuleSource>().
                template moduleStateChanged<T>(state, *static_cast<T *>(this));
    }

};

template<typename T, typename ... Args>
using NodeModuleDependent = NodeModuleConfigDependent<T, INodeModule::Config, Args...>;


#endif //BASYCO_NODEMODULE_H
