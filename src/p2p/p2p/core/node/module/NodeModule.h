#include <utility>

//
// Created by Krzysztof Tulidowicz on 20.10.17.
//

#ifndef BSC_NODEMODULE_H
#define BSC_NODEMODULE_H


#include <p2p/core/node/module/logic/ILogicModule.h>
#include <p2p/core/dependency/IDependencyManaged.h>
#include <p2p/core/dependency/DependencyManaged.h>
#include <p2p/core/node/configuration/ConfigurationManager.h>
#include "p2p/core/node/INode.h"
#include "INodeModule.h"


#include <p2p/core/node/module/logic/ModuleSource.h>

namespace bsc {
    class NodeModule : public ILogicModule {
    private:
        const ModuleIdType moduleId;

    public:
        using Configuration = IConfig;

        //@todo pure or not?
        void initialize() override{};

        void ready() override {};

        void shutdown() override {};

        void run() override;

    protected:
        ConfigurationManager& getConfigurationManager();

    public:
        const ModuleIdType& getModuleId() const override;

        NodeModule(INode& node, ModuleIdType moduleId) : ILogicModule(node), moduleId(std::move(moduleId)) {}
    };

//@todo maybe remove DependencyManaged, I don't think it's required. modules are now async, they don't have to be sorted
template<typename T, typename ... Args>
    class NodeModuleDependent : public bsc::NodeModule, public DependencyManaged<T> {
    public:
        auto& getOwnSubModule() {
            return getSubModule<T>();
        }

    private:
        template<typename T1, typename... Args1>
        void checkAndAddModules() {
            if (!node.hasModule<T1>()) {
                node.addModule<T1>();
                logger.debug(std::string("MODULE NOT FOUND, ADDING MODULE") + typeid(T1).name());
            }
        if constexpr (sizeof...(Args1) > 0) {
            checkAndAddModules<Args1...>();
        }
    }

public:

    NodeModuleDependent(INode &node, const ModuleIdType &moduleId) : NodeModule(node, moduleId) {
        this->template setRequired<Args...>();
        if constexpr (sizeof...(Args) > 0) {
            checkAndAddModules<Args...>();
        }
    }

    auto &configuration() {
        return getConfiguration<T>();
    }

    void initializeConfiguration() final {
        loadConfiguration();
    }

protected:
    void changeModuleState(const ModuleState &state) override {
        ILogicModule::changeModuleState(state);
        node.getLogicManager().template requireSource<ModuleSource>().
                template moduleStateChanged<T>(state, *static_cast<T *>(this));
    }

public:
    void saveConfiguration() override {
        //@todo replace getConfigId() with getModuleId() from module. Modules should have unique ids.
        node.getConfigurationManager().save<typename T::Configuration>(this->getModuleId(),
                                                                       configuration());
    }

    void loadConfiguration() override {
        auto loaded = node.getConfigurationManager().template load<typename T::Configuration>(
                this->getModuleId());
        logger.debug(std::string("Checking configuration for class: ") + typeid(T).name());
        if (loaded) {
            configuration() = *loaded;
            logger.debug("Configuration found, loading");
        } else {
            logger.debug("Failed to load configuration");
        }
    }
};
}

#endif //BSC_NODEMODULE_H
