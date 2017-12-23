//
// Created by stilgar on 10.12.17.
//

#include <p2p/log/Logger.h>
#include <p2p/utils/crypto.h>
#include <p2p/network/protocol/context/NodeContext.h>
#include "InternalStorage.h"

void InternalStorage::store(const ResourceId &checksum, const size_t &size, const PathType &sourcePath) {

    auto realChecksum = calculateSha1OfFile(fs::path(sourcePath));
    auto realFileSize = fs::file_size(sourcePath);
    if (realChecksum == checksum && realFileSize == size) {
        //FILE IS OK
        LOGGER("CHECKSUM IS OK, COPYING TO INTERNAL STORAGE");

        ///@TODO handle directories
        fs::copy(sourcePath, getResourcePath(getResourceId(checksum, size)));
    }

}

bool
InternalStorage::restore(const ResourceId &checksum, const size_t &size, const PathType &destinationPath) {

    bool restoreFile = false;
    if (fs::exists(destinationPath)) {
        auto realChecksum = calculateSha1OfFile(fs::path(destinationPath));
        auto realFileSize = fs::file_size(destinationPath);
        restoreFile = !(realChecksum == checksum && realFileSize == size);
    } else {
        restoreFile = true;
    }

    ///@todo more checks, for example, if the file is in the internal storage or should be acquired from another storage

    if (restoreFile) {
        fs::copy(getResourcePath(getResourceId(checksum, size)), destinationPath);
    }

}

void InternalStorage::update(const ResourceId &checksum, const size_t &size, const PathType &sourcePath) {

}

void InternalStorage::sync(const NodeIdType &nodeID) {

}

void InternalStorage::initStorage() {

    auto &node = NodeContext::getNodeFromActiveContext();

    storagePath = "/tmp/storage";
    storagePath /= node.getNodeInfo().getNodeId();
    storagePath /= repository->getRepositoryId();
    storagePath /= "internal";


    if (!fs::exists(this->storagePath)) {
        if (fs::create_directories(storagePath)) {
            LOGGER("STORAGE INITIALIZED")
        } else {
            LOGGER("FAILED TO INITIALIZE STORAGE");
        }
    }

}

InternalStorage::InternalStorage(IRepository *r) : IStorage(r) {
    initStorage();
}

fs::path InternalStorage::getResourcePath(const IStorage::ResourceId &resourceId) {
    return storagePath / resourceId;
}

bool InternalStorage::hasResource(const IStorage::ResourceId &resourceId) {
    return fs::exists(getResourcePath(resourceId));
}

//InternalStorage::InternalStorage(const std::string &storageId) /*: storageId(storageId)*/ {
//    initStorage();
//}
