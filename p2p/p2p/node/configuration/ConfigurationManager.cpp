#include <utility>

//
// Created by stilgar on 21.08.17.
//

#include <fstream>
#include "ConfigurationManager.h"
#include <cereal/archives/xml.hpp>



//void ConfigurationManager::save(const ConfigurationManager::IdType &id, std::shared_ptr<IConfig> config) {
//    //@todo implement saving config to file
////      fs::path configPath("~/.basyco/");
//
//    fs::path fname;//("test");
//    fname = this->filenameFromId(id);
//    fs::path filePath = getConfigPath() / fname;
//    fs::create_directories(filePath.parent_path());
//    LOGGER(filePath);
//    std::ofstream os(filePath);
//    cereal::XMLOutputArchive archive(os);
//    archive << config;
//
//}

//std::shared_ptr<IConfig> ConfigurationManager::loadRaw(const ConfigurationManager::IdType &id) {
//    std::shared_ptr<IConfig> config;
//    fs::path fname = this->filenameFromId(id);
//    fs::path filePath = getConfigPath() / fname;
//    if (fs::exists(filePath)) {
//        std::ifstream is(filePath);
//        cereal::XMLInputArchive archive(is);
//        archive >> config;
//
//    }
//    return config;
//
//
//}

ConfigurationManager::ConfigurationManager(std::filesystem::path rootPath) : rootPath(std::move(rootPath)) {}

const std::filesystem::path &ConfigurationManager::getRootPath() const {
    return rootPath;
}

void ConfigurationManager::setRootPath(const std::filesystem::path &rootPath) {
    ConfigurationManager::rootPath = rootPath;
    initializeRootPath();
}

fs::path ConfigurationManager::filenameFromId(const ConfigurationManager::IdType &id) {
    //@todo do something with it
    fs::path ret = (id + ".cfg");
    return ret;
}

fs::path ConfigurationManager::getConfigPath() {
    //@todo replace "config" with variable
    return rootPath / fs::path("config");
}

fs::path ConfigurationManager::getDataPath() {
    //@todo replace "data" with variable
    return rootPath / fs::path("data");
}

