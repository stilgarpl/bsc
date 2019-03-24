//
// Created by stilgar on 10.12.17.
//

#include <p2p/log/Logger.h>
#include <p2p/utils/crypto.h>
#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/nodeNetworkModule/NodeNetworkModule.h>
#include <repo/repository/storage/network/packet/StorageQuery.h>
#include <p2p/modules/filesystem/FilesystemModule.h>
#include "InternalStorage.h"
#include "StorageResourceIdentificator.h"


void InternalStorage::store(const ResourceId &checksum, const size_t &size, const PathType &sourcePath) {

    auto realChecksum = calculateSha1OfFile(fs::path(sourcePath));
    auto realFileSize = fs::file_size(sourcePath);
    if (realChecksum == checksum && realFileSize == size) {
        //FILE IS OK
        LOGGER("CHECKSUM IS OK, COPYING TO INTERNAL STORAGE");
        fs::copy_options options = fs::copy_options::none;
        //check if file is already in storage
        if (fs::exists(getResourcePath(getResourceId(checksum, size)))) {
            auto hasChecksum = calculateSha1OfFile(getResourcePath(getResourceId(checksum, size)));
            if (hasChecksum != checksum) {
                //file in storage was corrupted!
                options = fs::copy_options::overwrite_existing;
            } else {
                options = fs::copy_options::skip_existing;
            }
        }
        fs::copy(sourcePath, getResourcePath(getResourceId(checksum, size)), options);
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
//    auto netModule = NodeContext::getNodeFromActiveContext().getModule<NodeNetworkModule>();
    auto fileModule = NodeContext::getNodeFromActiveContext().getModule<FilesystemModule>();
//    StorageQuery::Request::Ptr req = StorageQuery::Request::getNew();
//    req->setRepositoryId(repository->getRepositoryId());
//    req->setObjectId(resourceId);
//    //@todo this will wait for response, potentially blocking Repository
//    auto responseMap = netModule->broadcastRequest(req);

    if (transferQueue == nullptr) {
        transferQueue = fileModule->transferQueue();
    }

    transferQueue->queueTransfer(std::make_shared<StorageResourceIdentificator>(
            repository->getRepositoryId(), resourceId), std::make_shared<StorageResourceIdentificator>(
            repository->getRepositoryId(), resourceId));
    transferQueue->start();

    return true;
}

void InternalStorage::restore(const ResourceId &resourceId, const PathType &destinationPath) {
    //@todo acquire before restore ?
    //@todo don't do restore if the target is identical or changed
    //@todo verify checksum and size (add way to get those from resource id?)
    auto resourcePath = getResourcePath(resourceId);
    LOGGER("IS:restore resP " + resourcePath.string() + " desP " + destinationPath)
    //@todo PathType should really be just path, it would save a lot of trouble.
//    fs::path desPath(destinationPath);
    fs::create_directories(fs::weakly_canonical(destinationPath).parent_path());
    fs::copy(resourcePath, destinationPath, fs::copy_options::overwrite_existing);
}

//InternalStorage::InternalStorage(const std::string &storageId) /*: storageId(storageId)*/ {
//    initStorage();
//}
