//
// Created by stilgar on 21.08.17.
//

#include "ConfigurationManager.h"

void ConfigurationManager::save(ConfigurationManager::IdType &id, std::shared_ptr<IConfig> config) {
    //@todo implement saving config to file
    //  fs::path configPath {"~/basyco/"};

    //  fs::path fname {"test"};
    //fs::path filePath = configPath /
}

std::shared_ptr<void> ConfigurationManager::load_void(const ConfigurationManager::IdType &id) {
    //@todo implemenet getting config from file
}
