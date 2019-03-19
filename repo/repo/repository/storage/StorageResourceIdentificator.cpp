//
// Created by stilgar on 03.01.18.
//

#include <p2p/node/context/NodeContext.h>
#include <repo/node/RepoModule.h>
#include "StorageResourceIdentificator.h"

std::shared_ptr<std::istream> StorageResourceIdentificator::getResourceInputStream() {

    auto repoModule = NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
    auto repo = repoModule->findRepository(repositoryId);
    if (repo != nullptr) {
        return repo->getStorage()->getResourceStream(objectId);
    } else {
        return nullptr;
    }
}

std::shared_ptr<std::ostream> StorageResourceIdentificator::getResourceOutputStream() {
    auto repoModule = NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
    auto repo = repoModule->findRepository(repositoryId);
    if (repo != nullptr) {
        return repo->getStorage()->getResourceStream(objectId);
    } else {
        return nullptr;
    }
}

uintmax_t StorageResourceIdentificator::getResourceSize() {
    auto repoModule = NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
    auto repo = repoModule->findRepository(repositoryId);
    if (repo != nullptr) {
        return fs::file_size(repo->getStorage()->getResourcePath(objectId));
    } else {
        //@todo throw error or sth?
        return 0;
    }
}

bool StorageResourceIdentificator::exists() {
    auto repoModule = NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
    auto repo = repoModule->findRepository(repositoryId);
    if (repo != nullptr) {
        return fs::exists(repo->getStorage()->getResourcePath(objectId));
    } else {

        return false;
    }
}

StorageResourceIdentificator::StorageResourceIdentificator(const IRepository::RepoIdType &repositoryId,
                                                           const IStorage::ResourceId &objectId) : repositoryId(
        repositoryId),
                                                                                                   objectId(objectId) {}
