//
// Created by stilgar on 10.12.17.
//

#include <core/log/Logger.h>
#include <core/utils/crypto.h>
#include <p2p/modules/filesystem/FilesystemModule.h>

#include <utility>
#include <p2p/node/context/NodeContext.h>
#include "InternalStorage.h"
#include "StorageResourceIdentificator.h"


void InternalStorage::store(const ResourceId& checksum, const size_t& size, const fs::path& sourcePath) {

    auto realChecksum = bsc::calculateSha1OfFile(fs::path(sourcePath));
    auto realFileSize = fs::file_size(sourcePath);
    if (realChecksum == checksum && realFileSize == size) {
        //file IS OK
        LOGGER("CHECKSUM IS OK, COPYING TO INTERNAL STORAGE");
        fs::copy_options options = fs::copy_options::none;
        //check if file is already in storage
        if (fs::exists(getResourcePath(getResourceId(checksum, size)))) {
            auto hasChecksum = bsc::calculateSha1OfFile(getResourcePath(getResourceId(checksum, size)));
            if (hasChecksum != checksum) {
                //file in storage was corrupted!
                options = fs::copy_options::overwrite_existing;
            } else {
                options = fs::copy_options::skip_existing;
            }
        }
        try {
            fs::copy(sourcePath, getResourcePath(getResourceId(checksum, size)), options);
        } catch (fs::filesystem_error &e) {
            LOGGER(e.what());
        }
    }

}

void InternalStorage::update(const ResourceId& checksum, const size_t& size, const fs::path& sourcePath) {

}

void InternalStorage::sync(const bsc::NodeIdType& nodeID) {

}

void InternalStorage::initStorage() {

    if (!fs::exists(this->storagePath)) {
        if (fs::create_directories(storagePath)) {
            LOGGER("STORAGE INITIALIZED")
        } else {
            LOGGER("FAILED TO INITIALIZE STORAGE");
        }
    }

}

fs::path InternalStorage::getResourcePath(const IStorage::ResourceId &resourceId) const {
    return storagePath / resourceId;
}

bool InternalStorage::hasResource(const IStorage::ResourceId &resourceId) const {
    return fs::exists(getResourcePath(resourceId));
}

std::shared_ptr<std::iostream> InternalStorage::getResourceStream(const ResourceId &resourceId) {
    //@todo make sure resource exists

    if (!fs::exists(getResourcePath(resourceId))) {
        std::fstream f(getResourcePath(resourceId), std::ios_base::out | std::ofstream::binary);
        f.close(); //file now always exists
    }

    //@todo separate input and output resources?
    return std::make_shared<std::fstream>(getResourcePath(resourceId),
                                          std::ios::in | std::ios::out | std::fstream::binary);
}

bool InternalStorage::acquireResource(const ResourceId &resourceId) {
//    auto netModule = NodeContext::getNodeFromActiveContext().getModule<NetworkModule>();
    auto fileModule = bsc::NodeContext::getNodeFromActiveContext().getModule<bsc::FilesystemModule>();
//    StorageQuery::Request::Ptr req = StorageQuery::Request::getNew();
//    req->setRepositoryId(repository->getRepositoryId());
//    req->setObjectId(resourceId);
//    //@todo this will wait for response, potentially blocking Repository
//    auto responseMap = netModule->broadcastRequest(req);
    LOGGER("acquiring resource " + resourceId)
    if (transferQueue == nullptr) {
        transferQueue = fileModule->transferQueue();
    }

    transferQueue->queueTransfer(std::make_shared<StorageResourceIdentificator>(
            getStorageId(), resourceId), std::make_shared<StorageResourceIdentificator>(
            getStorageId(), resourceId));
    transferQueue->start();

    return true;
}

void InternalStorage::restore(const ResourceId& resourceId, const fs::path& destinationPath) {
    //@todo error handling if resourceId is not in storage
    //@todo acquire before restore ?
    //@todo don't do restore if the target is identical or changed
    //@todo verify checksum and size (add way to get those from resource id?)
    auto resourcePath = getResourcePath(resourceId);
    LOGGER("IS:restore resP " + resourcePath.string() + " desP " + destinationPath.string())

    fs::create_directories(fs::weakly_canonical(destinationPath).parent_path());
    try {
        fs::copy(resourcePath, destinationPath, fs::copy_options::overwrite_existing);
    } catch (fs::filesystem_error &e) {
        LOGGER(e.what());
    }
}

bsc::TransferManager::TransferQueuePtr InternalStorage::getTransferQueue() {
    return transferQueue;
}

InternalStorage::InternalStorage(StorageId storageId, fs::path path) : IStorage(std::move(storageId)),
                                                                       storagePath(path) {
    initStorage();
}

