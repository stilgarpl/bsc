//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_CONFIGURATIONMANAGER_H
#define BASYCO_CONFIGURATIONMANAGER_H

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#include "IConfig.h"


class ConfigurationManager {
public:
    typedef std::string IdType;
private:
    fs::path rootPath;


public:
    fs::path getConfigPath() {
        return rootPath / fs::path("config");
    }

protected:
    fs::path filenameFromId(const IdType &id) {
        //@todo do something with it
        fs::path ret = (id + ".cfg");
        return ret;
    }

public:
    void save(const IdType &id, std::shared_ptr<IConfig> config);


    ///@todo Concepts: T extends IConfig
    template<typename T>
    std::shared_ptr<T> load(const IdType &id) {
        return std::static_pointer_cast<T>(load_void(id));
    };
protected:
    ///@todo why not IConfig
    std::shared_ptr<void> load_void(const IdType &id);

private:
    void initializeRootPath() {
        fs::create_directories(rootPath);
    }
public:
    ConfigurationManager(const std::experimental::filesystem::path &rootPath);

    ConfigurationManager();

    const std::experimental::filesystem::path &getRootPath() const;

    void setRootPath(const std::experimental::filesystem::path &rootPath);
};


#endif //BASYCO_CONFIGURATIONMANAGER_H
