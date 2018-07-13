//
// Created by stilgar on 10.12.17.
//

#include <p2p/log/Logger.h>
#include <p2p/utils/crypto.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/NodeContext.h>
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

    auto resourceId = getResourceId(checksum, size);
    auto resourcePath = getResourcePath(resourceId);
    if (!fs::exists(resourcePath) || !restoreFile) {
        //storage does not have this object, acquire

        auto netModule = NodeContext::getNodeFromActiveContext().getModule<NodeNetworkModule>();
        StorageQuery::Request::Ptr req = StorageQuery::Request::getNew();
        req->setRepositoryId(repository->getRepositoryId());
        req->setObjectId(resourceId);
        ///@todo this will wait for response, potentially blocking Repository
        auto response = netModule->broadcastPacket(req);
        TransferManager::LocalTransferDescriptorPtr transfer = nullptr;

        for (auto &&item : response) {
            LOGGER("node " + item.first + " replied with " + std::to_string(item.second->isExists()));
            if (item.second->isExists()) {
                auto fileModule = NodeContext::getNodeFromActiveContext().getModule<FilesystemModule>();
                transfer = fileModule->remoteGetStream(item.first, std::make_shared<StorageResourceIdentificator>(
                        repository->getRepositoryId(), resourceId), std::make_shared<StorageResourceIdentificator>(
                        repository->getRepositoryId(), resourceId));

                break;
            }
        }
        if (transfer != nullptr) {
            transfer->wait();
            restoreFile = true;
        }
    }



    if (restoreFile) {
        ///@todo catch filesystem exceptions or use no except version
        fs::copy(resourcePath, destinationPath);
    }

    return restoreFile;

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
    ///@todo make sure resource exists

    if (!fs::exists(getResourcePath(resourceId))) {
        std::fstream f(getResourcePath(resourceId), std::ios_base::out | std::ofstream::binary);
        f.close(); //file now always exists
    }

    ///@todo separate input and output resources?
    return std::make_shared<std::fstream>(getResourcePath(resourceId),
                                          std::ios::in | std::ios::out | std::fstream::binary);
}

//InternalStorage::InternalStorage(const std::string &storageId) /*: storageId(storageId)*/ {
//    initStorage();
//}
