//
// Created by stilgar on 17.05.18.
//

#include "ConfigurationModule.h"

ConfigurationModule::ConfigurationModule(INode &node) : NodeModuleDependent(node) {}


void ConfigurationModule::save(const ConfigurationManager::IdType &id, std::shared_ptr<IConfig> config) {
    manager.save(id, config);
}

void ConfigurationModule::initialize() {
    NodeModule::initialize();
    manager.setRootPath(fs::path("/tmp/basyco") / node.getNodeInfo().getNodeId());
}

void ConfigurationModule::setupActions(LogicManager &logicManager) {

}

bool ConfigurationModule::assignActions(LogicManager &logicManager) {
    return true;
}

bool ConfigurationModule::setupSources(LogicManager &logicManager) {
    return true;
}
