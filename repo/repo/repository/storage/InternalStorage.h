//
// Created by stilgar on 10.12.17.
//

#ifndef BASYCO_INTERNALSTORAGE_H
#define BASYCO_INTERNALSTORAGE_H


#include "IStorage.h"
#include <filesystem>
#include <p2p/modules/filesystem/transfer/TransferManager.h>
namespace bsc {
    namespace fs = std::filesystem;

    class InternalStorage : public IStorage {

    private:
        // std::string storageId;
        const fs::path storagePath;
        bsc::TransferManager::TransferQueuePtr transferQueue = nullptr;
    public:
        void store(const ResourceId& checksum, const size_t& size, const fs::path& sourcePath) override;

        void update(const ResourceId& checksum, const size_t& size, const fs::path& sourcePath) override;

        void sync(const bsc::NodeIdType& nodeID) override;

        void initStorage();

        explicit InternalStorage(StorageId storageId, fs::path path);

//
        std::shared_ptr<std::iostream> getResourceStream(const ResourceId& resourceId) override;
//    InternalStorage(const std::string &storageId);


        [[nodiscard]] fs::path getResourcePath(const ResourceId& resourceId) const override;

        [[nodiscard]] bool hasResource(const ResourceId& resourceId) const override;

        bool acquireResource(const ResourceId& resourceId) override;

        void restore(const ResourceId& resourceId, const fs::path& destinationPath) override;

        ~InternalStorage() override = default;

        bsc::TransferManager::TransferQueuePtr getTransferQueue() override;
    };
}

#endif //BASYCO_INTERNALSTORAGE_H
