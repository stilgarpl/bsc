//
// Created by stilgar on 21.08.17.
//

#include <fstream>
#include "ConfigurationManager.h"
#include <cereal/archives/xml.hpp>
#include <p2p/log/Logger.h>


void ConfigurationManager::save(const ConfigurationManager::IdType &id, std::shared_ptr<IConfig> config) {
    //@todo implement saving config to file
//      fs::path configPath("~/.basyco/");

    fs::path fname;//("test");
    fname = this->filenameFromId(id);
    fs::path filePath = getConfigPath() / fname;
    fs::create_directories(filePath.parent_path());
    LOGGER(filePath);
    std::ofstream os(filePath);
    cereal::XMLOutputArchive archive(os);
    archive << config;

}

std::shared_ptr<void> ConfigurationManager::load_void(const ConfigurationManager::IdType &id) {
    std::shared_ptr<IConfig> config;
    fs::path fname = this->filenameFromId(id);
    fs::path filePath = getConfigPath() / fname;
    if (fs::exists(filePath)) {
        std::ifstream is(filePath);
        cereal::XMLInputArchive archive(is);
        archive >> config;

    }
    return config;


}

ConfigurationManager::ConfigurationManager(const std::experimental::filesystem::path &rootPath) : rootPath(rootPath) {}

ConfigurationManager::ConfigurationManager() {}

const std::experimental::filesystem::path &ConfigurationManager::getRootPath() const {
    return rootPath;
}

void ConfigurationManager::setRootPath(const std::experimental::filesystem::path &rootPath) {
    ConfigurationManager::rootPath = rootPath;
    initializeRootPath();
}