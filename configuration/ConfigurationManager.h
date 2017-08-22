//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_CONFIGURATIONMANAGER_H
#define BASYCO_CONFIGURATIONMANAGER_H

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#include "IConfig.h"

class ConfigurationManager {
    typedef std::string IdType;
    fs::path configPath{"~/basyco/"};

protected:
    std::string filenameFromId(const IdType &id) {
        //@todo do something with it
        return id;
    }

public:
    void save(IdType &id, std::shared_ptr<IConfig> config);

    template<typename T>
    std::shared_ptr<T> load(const IdType &id) {
        return std::static_pointer_cast<T>(load_void(id));
    };
protected:
    std::shared_ptr<void> load_void(const IdType &id);
};


#endif //BASYCO_CONFIGURATIONMANAGER_H
