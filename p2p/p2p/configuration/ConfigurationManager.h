//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_CONFIGURATIONMANAGER_H
#define BASYCO_CONFIGURATIONMANAGER_H

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#include "IConfig.h"

//@todo should this be a singleton? nah, it should be per node
class ConfigurationManager {
    typedef std::string IdType;
    fs::path configPath;

protected:
    fs::path filenameFromId(const IdType &id) {
        //@todo do something with it
        fs::path ret = (id + ".cfg");
        return ret;
    }

public:
    void save(const IdType &id, std::shared_ptr<IConfig> config);

    ///@todo why not IConfig
    template<typename T>
    std::shared_ptr<T> load(const IdType &id) {
        return std::static_pointer_cast<T>(load_void(id));
    };
protected:
    std::shared_ptr<void> load_void(const IdType &id);

private:
    void initializeManager() {
        configPath = "/tmp/basyco/";
        //create directory if not exist
        fs::create_directories(configPath);

    }
public:
    ConfigurationManager();
};


#endif //BASYCO_CONFIGURATIONMANAGER_H
