//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_CONFIGURATIONMANAGER_H
#define BASYCO_CONFIGURATIONMANAGER_H

#include <filesystem>
#include <fstream>
#include <p2p/log/Logger.h>

namespace fs = std::filesystem;

#include "IConfig.h"

//@todo consider changing name to IOManager or sth like that, because it doesn't just do configuration but everything that goes into private node directory
class ConfigurationManager {
public:
    typedef IConfig::IdType IdType;
private:
    fs::path rootPath;


protected:
    fs::path getConfigPath();

    fs::path getDataPath();

protected:
    fs::path filenameFromId(const IdType &id);

public:
//    void save(const IdType &id, std::shared_ptr<IConfig> config);

//@todo maybe unify save(load) and saveData (loadData) functions

    template<typename ConfigType>
    void save(const IdType &id, ConfigType &config) {
        fs::path fname;//("test");
        fname = this->filenameFromId(id);
        fs::path filePath = getConfigPath() / fname;
        fs::create_directories(filePath.parent_path());
        LOGGER(filePath);
        std::ofstream os(filePath);
        cereal::JSONOutputArchive archive(os);
        archive << config;
    }

    template<typename DataType /* @todo Concept: DataType is serializable*/>
    void saveData(const fs::path &fname, const DataType &data) {
        fs::path filePath = getDataPath() / fname;
        fs::create_directories(filePath.parent_path());
        std::ofstream os(filePath);
        cereal::XMLOutputArchive archive(os);
        archive << data;
    }

    template<typename DataType /* @todo Concept: DataType is serializable*/>
    DataType loadData(const fs::path &fname) {
        DataType data;
        fs::path filePath = getDataPath() / fname;
        if (fs::exists(filePath)) {
            std::ifstream is(filePath);
            cereal::XMLInputArchive archive(is);
            archive >> data;

        } else {
            //@todo throw file not found exception
        }
        return data;
    }


    //@todo Concept: T extends IConfig
    template<typename ConfigType>
    std::optional<ConfigType> load(const IdType &id) {
        std::optional<ConfigType> config;
        fs::path fname = this->filenameFromId(id);
        fs::path filePath = getConfigPath() / fname;
        if (fs::exists(filePath)) {
            std::ifstream is(filePath);
            cereal::JSONInputArchive archive(is);
            archive >> *config;

        }
        return config;
    }
protected:
//    std::shared_ptr<IConfig> loadRaw(const IdType &id);

private:
    void initializeRootPath() {
        fs::create_directories(rootPath);
    }
public:
    explicit ConfigurationManager(std::filesystem::path rootPath);

    ConfigurationManager() = default;

    const std::filesystem::path &getRootPath() const;

    void setRootPath(const std::filesystem::path &rootPath);
};


#endif //BASYCO_CONFIGURATIONMANAGER_H
