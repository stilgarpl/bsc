//
// Created by stilgar on 20.10.17.
//

#ifndef BASYCO_NODEMODULE_H
#define BASYCO_NODEMODULE_H


#include <p2p/logic/ILogicModule.h>
#include <p2p/dependency/IDependencyManaged.h>
#include <p2p/dependency/DependencyManaged.h>
#include <p2p/modules/configuration/ConfigurationManager.h>
#include "INode.h"
#include "INodeModule.h"

class NodeModule : public INodeModule {

protected:
    INode &node;
public:
    ///@todo pure or not?
    virtual void initialize() {};

    explicit NodeModule(INode &node) : INodeModule(node.getLogicManager()), node(node) {}

    void run() override {
        //do nothing, just so modules do not have to implement this if they don't want to
    }

protected:
    ConfigurationManager &getConfigurationManager() {
        return node.getConfigurationManager();
    }
};

/**
 * NodeModule with automatic dependency management
 * @tparam T actual module type, i.e. class SomeModule : public NodeModuleDependent<SomeModule>
 * @tparam Args optional dependent classes. it could also be set by using setRequired<> method or addRequiredDependency<>
 */
template<typename T, typename ... Args>
class NodeModuleDependent : public NodeModule, public DependencyManaged<T> {
private:
//    typename T::Config _config;
    Config _config;
public:
    IConfig &configuration() override {
        return _config;
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
    explicit NodeModuleDependent(INode &node) : NodeModule(node) {
        this->template setRequired<Args...>();
        if constexpr (sizeof...(Args) > 0) {
            checkAndAddModules<Args...>();
        }
    };

    void initializeConfiguration() override {
        auto loaded = node.getConfigurationManager().load<Config>(configuration().getConfigId());
        if (loaded != nullptr)
            configuration() = *loaded;
    }
};

template<typename T, typename ConfigType, typename ... Args>
class NodeModuleConfigDependent : public NodeModule, public DependencyManaged<T> {
public:
//    typename T::Config _config;
    typedef ConfigType Config;
private:
    Config _config;
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
        auto loaded = node.getConfigurationManager().load<Config>(configuration().getConfigId());
        LOGGER(std::string("Checking configuration for class: ") + typeid(T).name());
        if (loaded != nullptr) {
            configuration() = *loaded;
            LOGGER("Configuration found, loading")
        }
    }

};

#endif //BASYCO_NODEMODULE_H
