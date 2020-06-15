//
// Created by stilgar on 19.08.2019.
//

#include "StorageFactory.h"

#include <utility>
#include "InternalStorage.h"
namespace bsc {

    IStoragePtr StorageFactory::create(const SelectorType& selector, const ArgumentType& storageName) const {
        //@todo implement actual identifiers and different storages
        LOGGER("creating storage with path " + (storagePath / storageName).string())
        return std::make_shared<InternalStorage>(storageName, storagePath / storageName);
    }

    StorageFactory::StorageFactory(fs::path storagePath) : storagePath(std::move(storagePath)) {}
}