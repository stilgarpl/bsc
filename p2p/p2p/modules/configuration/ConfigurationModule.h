//
// Created by stilgar on 17.05.18.
//

#ifndef BASYCO_CONFIGURATIONMODULE_H
#define BASYCO_CONFIGURATIONMODULE_H


#include <p2p/node/NodeModule.h>
#include <p2p/modules/basic/BasicModule.h>
#include "ConfigurationManager.h"

class ConfigurationModule : public NodeModuleDependent<ConfigurationModule, BasicModule> {
private:
    ConfigurationManager manager;
public:
    ConfigurationModule(INode &node);

    void save(const ConfigurationManager::IdType &id, std::shared_ptr<IConfig> config);


    //@todo Concepts: T extends IConfig
    template<typename T>
    std::shared_ptr<T> load(const ConfigurationManager::IdType &id) {
        //@todo make new or ...
        ///if load failed, create defaults

        auto ret = manager.load<T>(id);
        if (ret) {
            return ret;
        } else {
            return std::make_shared<T>();
        }
    }

    void initialize() override;

    void setupActions(ILogicModule::SetupActionHelper &actionHelper) override;

    bool assignActions(ILogicModule::AssignActionHelper &actionHelper) override;

    bool setupSources(ILogicModule::SetupSourceHelper &sourceHelper) override;

};


#endif //BASYCO_CONFIGURATIONMODULE_H
