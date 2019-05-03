//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_CONFIGURATIONMANAGER_H
#define BASYCO_CONFIGURATIONMANAGER_H

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

#include "IConfig.h"

//@todo consider changing name to IOManager or sth like that, because it doesn't just do configuration but everything that goes into private node directory
class ConfigurationManager {
public:
    typedef IConfig::IdType IdType;
private:
    fs::path rootPath;


protected:
    fs::path getConfigPath() {
        //@todo replace "config" with variable
        return rootPath / fs::path("config");
    }

    fs::path getDataPath() {
        //@todo replace "data" with variable
        return rootPath / fs::path("data");
    }


protected:
    fs::path filenameFromId(const IdType &id) {
        //@todo do something with it
        fs::path ret = (id + ".cfg");
        return ret;
    }

public:
    void save(const IdType &id, std::shared_ptr<IConfig> config);

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
    template<typename T>
    std::shared_ptr<T> load(const IdType &id) {
        return std::static_pointer_cast<T>(load_void(id));
    }
protected:
    //@todo why not IConfig
    std::shared_ptr<void> load_void(const IdType &id);

private:
    void initializeRootPath() {
        fs::create_directories(rootPath);
    }
public:
    explicit ConfigurationManager(const std::filesystem::path &rootPath);

    ConfigurationManager();

    const std::filesystem::path &getRootPath() const;

    void setRootPath(const std::filesystem::path &rootPath);
};


#endif //BASYCO_CONFIGURATIONMANAGER_H
