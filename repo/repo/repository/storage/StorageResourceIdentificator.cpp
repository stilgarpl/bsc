//
// Created by stilgar on 03.01.18.
//

#include <p2p/core/node/context/NodeContext.h>
#include <repo/node/RepoModule.h>

#include <utility>
#include "StorageResourceIdentificator.h"

namespace bsc {

    std::shared_ptr<std::istream> StorageResourceIdentificator::getResourceInputStream() {

        auto repoModule = bsc::NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
        auto storage = repoModule->findStorage(storageId);
        if (storage != nullptr) {
            return storage->getResourceStream(objectId);
        } else {
            return nullptr;
        }
    }

    std::shared_ptr<std::ostream> StorageResourceIdentificator::getResourceOutputStream() {
        auto repoModule = bsc::NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
        auto storage = repoModule->findStorage(storageId);
        if (storage != nullptr) {
            return storage->getResourceStream(objectId);
        } else {
            return nullptr;
        }
    }

    uintmax_t StorageResourceIdentificator::getResourceSize() {
        auto repoModule = bsc::NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
        auto storage = repoModule->findStorage(storageId);
        if (storage != nullptr) {
            return fs::file_size(storage->getResourcePath(objectId));
        } else {
            //@todo throw error or sth?
            return 0;
        }
    }

    bool StorageResourceIdentificator::exists() {
        LOGGER("storage resource exists")
        auto repoModule = bsc::NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
        auto storage = repoModule->findStorage(storageId);
        if (storage != nullptr) {
            LOGGER("storage isn't null")
            LOGGER(storage->getResourcePath(objectId).string());
            return fs::exists(storage->getResourcePath(objectId));
        } else {
            LOGGER("storage is null")
            return false;
        }
    }

    StorageResourceIdentificator::StorageResourceIdentificator(IStorage::StorageId storageId,
                                                               IStorage::ResourceId objectId) : storageId(std::move(
            storageId)),
                                                                                                objectId(std::move(
                                                                                                        objectId)) {}
}