//
// Created by stilgar on 17.05.18.
//

#include "ConfigurationModule.h"

ConfigurationModule::ConfigurationModule(INode &node) : NodeModuleDependent<ConfigurationModule, BasicModule>(node) {}


void ConfigurationModule::save(const ConfigurationManager::IdType &id, std::shared_ptr<IConfig> config) {
    manager.save(id, config);
}

void ConfigurationModule::initialize() {
    NodeModule::initialize();
    manager.setRootPath(fs::path("/tmp/basyco") / node.getNodeInfo().getNodeId());
}

void ConfigurationModule::setupActions(ILogicModule::SetupActionHelper &actionHelper) {

}

bool ConfigurationModule::assignActions(ILogicModule::AssignActionHelper &actionHelper) {
    return true;
}

bool ConfigurationModule::setupSources(ILogicModule::SetupSourceHelper &sourceHelper) {
    return true;
}
