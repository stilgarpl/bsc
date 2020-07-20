//
// Created by Krzysztof Tulidowicz on 20.10.17.
//

#include "NodeModule.h"

void bsc::NodeModule::run() {
    //do nothing, just so modules do not have to implement this if they don't want to
    waitForStop();
}
bsc::ConfigurationManager& bsc::NodeModule::getConfigurationManager() {
    return node.getConfigurationManager();
}
const bsc::INodeModule::ModuleIdType& bsc::NodeModule::getModuleId() const {
    return moduleId;
}
