//
// Created by Krzysztof Tulidowicz on 21.08.17.
//

#ifndef BSC_CONFIGURATIONMANAGER_H
#define BSC_CONFIGURATIONMANAGER_H

#include "DataFileNotFoundException.h"
#include <core/utils/cereal_include.h>
#include <filesystem>
#include <fstream>
#include <log/log/Logger.h>

namespace bsc {
    namespace fs = std::filesystem;

//@todo consider changing name to IOManager or sth like that, because it doesn't just do configuration but everything that goes into private node directory
    class ConfigurationManager {
    public:
        typedef std::string IdType;
    private:
        fs::path rootPath;


    public:
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
        ::std::filesystem::create_directories(filePath.parent_path());
        LOGGER("saving: " + filePath.string());
        std::ofstream os(filePath);
        cereal::JSONOutputArchive archive(os);
        archive << cereal::make_nvp("configuration", config);
    }

    template<typename ConfigType>
    std::optional<ConfigType> load(const IdType &id) {
        std::optional<ConfigType> config;
        fs::path fname = this->filenameFromId(id);
        fs::path filePath = getConfigPath() / fname;
        LOGGER("loading: " + filePath.string());
        if (::std::filesystem::exists(filePath)) {
            ConfigType temp;
            try {
                std::ifstream is(filePath);
                cereal::JSONInputArchive archive(is);
                archive >> temp;
                config = temp;
            } catch (...) {
                LOGGER("failed to load file. " + filePath.string())
            }

        }
        return config;
    }

    fs::path getFullDataPath(const fs::path &fname) {
        return fname.is_relative() ? getDataPath() / fname : fname;
    }

//@todo archive type? now it's always xml.
    template<typename DataType /* @todo Concept: DataType is serializable*/>
    void saveData(const fs::path &fname, const DataType &data) {
        fs::path filePath = getFullDataPath(fname);
            ::std::filesystem::create_directories(filePath.parent_path());
        std::ofstream os(filePath);
        cereal::XMLOutputArchive archive(os);
        archive << data;
    }

    //@todo archive type? now it's always xml. maybe use type_trait for DataType?
    template<typename DataType /* @todo Concept: DataType is serializable*/>
    DataType loadData(const fs::path &fname) {
        DataType data;
        fs::path filePath = getFullDataPath(fname);
        if (::std::filesystem::exists(filePath)) {
            std::ifstream is(filePath);
            cereal::XMLInputArchive archive(is);
            archive >> data;

        } else {
            throw DataFileNotFoundException("Data file to load not found : " + filePath.string());
        }
        return data;
    }


protected:
//    std::shared_ptr<IConfig> loadRaw(const IdType &id);

private:
    void initializeRootPath() {
        ::std::filesystem::create_directories(rootPath);
    }

public:
    explicit ConfigurationManager(std::filesystem::path rootPath);

    ConfigurationManager() = default;

    const std::filesystem::path &getRootPath() const;

    void setRootPath(const std::filesystem::path &rootPath);
};
}


#endif //BSC_CONFIGURATIONMANAGER_H
