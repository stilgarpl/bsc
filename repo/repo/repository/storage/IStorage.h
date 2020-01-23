//
// Created by stilgar on 10.12.17.
//

#ifndef BSC_ISTORAGE_H
#define BSC_ISTORAGE_H


#include <repo/journal/JournalTypes.h>
#include <p2p/node/NodeInfo.h>
#include <filesystem>
#include <utility>
#include <p2p/modules/filesystem/transfer/TransferManager.h>

namespace bsc {
    namespace fs = std::filesystem;


    class IStorage {
    public:
        using StorageId = std::string;
        using ResourceId = std::string;
        //@todo export typedefs to one file included from journal and storage.

        static ResourceId getResourceId(const ResourceId& checksum, const size_t& size) {
            return std::to_string(size) + "_" + checksum;
        }

    protected:
        StorageId storageId;

    public:
//@fixme store, restore, update -> checksum is not of ResourceId type! ResourceId is generated from checksum and size.
//@todo store should only take path and return resourceId <--- THIS
        virtual void store(const ResourceId& checksum, const size_t& size, const fs::path& path) = 0;

        virtual void restore(const ResourceId& resourceId, const fs::path& path) = 0;

        //@todo I don't know if this method is actually useful. remove it if it's really not needed
        virtual void update(const ResourceId& checksum, const size_t& size, const fs::path& path) = 0;

        //syncs whole repository to corresponding repository from other node
        virtual void sync(const bsc::NodeIdType& nodeID) = 0;


        //@todo remove, I think - this is required for resource transfer between nodes, but streams should do that.
        //@todo or not remove, but make it protected
        [[nodiscard]] virtual fs::path getResourcePath(const ResourceId& resourceId) const = 0;

        virtual std::shared_ptr<std::iostream> getResourceStream(const ResourceId& resourceId) = 0;

        [[nodiscard]] virtual bool hasResource(const ResourceId& resourceId) const = 0;

        virtual bool acquireResource(const ResourceId& resourceId) = 0;

        explicit IStorage(StorageId storageId) : storageId(std::move(storageId)) {}

        virtual ~IStorage() = default;

        virtual bsc::TransferManager::TransferQueuePtr getTransferQueue() = 0;

        [[nodiscard]] const StorageId& getStorageId() const {
            return storageId;
        }

    };

    using IStoragePtr = std::shared_ptr<IStorage>;

}
#endif //BSC_ISTORAGE_H
