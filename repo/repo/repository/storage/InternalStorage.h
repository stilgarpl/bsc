//
// Created by stilgar on 10.12.17.
//

#ifndef BASYCO_INTERNALSTORAGE_H
#define BASYCO_INTERNALSTORAGE_H


#include "IStorage.h"

#include <filesystem>

namespace fs = std::filesystem;

class InternalStorage : public IStorage {

private:
    // std::string storageId;
    //@todo make configuration() with this value?
    fs::path storagePath = "/tmp/internal/";
public:
    void store(const ResourceId &checksum, const size_t &size, const PathType &sourcePath) override;

    bool restore(const ResourceId &checksum, const size_t &size, const PathType &destinationPath) override;

    void update(const ResourceId &checksum, const size_t &size, const PathType &sourcePath) override;

    void sync(const NodeIdType &nodeID) override;

    void initStorage();

//
    std::shared_ptr<std::iostream> getResourceStream(const ResourceId &resourceId) override;
//    InternalStorage(const std::string &storageId);

    InternalStorage(IRepository *r);

    fs::path getResourcePath(const ResourceId &resourceId) const override;

    bool hasResource(const ResourceId &resourceId) const override;

    bool acquireResource(const ResourceId &resourceId) override;

    void restore(const ResourceId &resourceId, const PathType &destinationPath) override;
};


#endif //BASYCO_INTERNALSTORAGE_H
