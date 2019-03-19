//
// Created by stilgar on 10.12.17.
//

#ifndef BASYCO_ISTORAGE_H
#define BASYCO_ISTORAGE_H


#include <repo/journal/JournalTypes.h>
#include <p2p/node/NodeInfo.h>
#include <repo/repository/IRepository.h>
#include <filesystem>

namespace fs = std::filesystem;


class IStorage {
public:

    using ResourceId = std::string;
    //@todo export typedefs to one file included from journal and storage.

    static ResourceId getResourceId(const ResourceId &checksum, const size_t &size) {
        return std::to_string(size) + "_" + checksum;
    }

protected:
    IRepository *repository;

public:
//@fixme store, restore, update -> checksum is not of ResourceId type! ResourceId is generated from checksum and size.
//@todo store should only take path and return resourceId
    virtual void store(const ResourceId &checksum, const size_t &size, const PathType &path) =0;

    virtual void restore(const ResourceId &resourceId, const PathType &path) = 0;

    //@todo I don't know if this method is actually useful. remove it if it's really not needed
    virtual void update(const ResourceId &checksum, const size_t &size, const PathType &path)=0;

    //syncs whole repository to corresponding repository from other node
    virtual void sync(const NodeIdType &nodeID)=0;


    //@todo remove, I think - this is required for resource transfer between nodes, but streams should do that.
    //@todo or not remove, but make it protected
    virtual fs::path getResourcePath(const ResourceId &resourceId) const =0;

    virtual std::shared_ptr<std::iostream> getResourceStream(const ResourceId &resourceId) =0;

    virtual bool hasResource(const ResourceId &resourceId) const =0;

    virtual bool acquireResource(const ResourceId &resourceId) = 0;

    explicit IStorage(IRepository *r) {
        this->repository = r;
    }

    virtual ~IStorage() = default;

};


#endif //BASYCO_ISTORAGE_H
