#include <utility>

//
// Created by Krzysztof Tulidowicz on 21.08.17.
//

#include "ConfigurationManager.h"
#include "core/utils/cereal_include.h"
#include <fstream>

// void ConfigurationManager::save(const ConfigurationManager::IdType &id, std::shared_ptr<IConfig> config) {
//    //@todo implement saving config to file
////      fs::path configPath("~/.bsc/");
//
//    fs::path fname;//("test");
//    fname = this->filenameFromId(id);
//    fs::path filePath = getConfigPath() / fname;
//    fs::create_directories(filePath.parent_path());
//    logger.debug(filePath);
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

bsc::ConfigurationManager::ConfigurationManager(std::filesystem::path rootPath) : rootPath(std::move(rootPath)) {}

const std::filesystem::path& bsc::ConfigurationManager::getRootPath() const {
    return rootPath;
}

void bsc::ConfigurationManager::setRootPath(const std::filesystem::path& rootPath) {
    ConfigurationManager::rootPath = rootPath;
    initializeRootPath();
}

fs::path bsc::ConfigurationManager::filenameFromId(const ConfigurationManager::IdType& id) {
    //@todo do something with it
    fs::path ret = (id + ".cfg");
    return ret;
}

fs::path bsc::ConfigurationManager::getConfigPath() {
    //@todo replace "config" with variable
    return rootPath / fs::path("config");
}

fs::path bsc::ConfigurationManager::getDataPath() {
    //@todo replace "data" with variable
    return rootPath / fs::path("data");
}

