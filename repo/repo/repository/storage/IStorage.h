//
// Created by stilgar on 10.12.17.
//

#ifndef BASYCO_ISTORAGE_H
#define BASYCO_ISTORAGE_H


#include <repo/journal/JournalTypes.h>
#include <p2p/network/node/NodeInfo.h>
#include <repo/repository/IRepository.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


class IStorage {
public:

    typedef std::string ResourceId;

    static ResourceId getResourceId(const JournalChecksumType &checksum, const size_t &size) {
        return std::to_string(size) + "_" + checksum;
    }

protected:
    IRepository *repository;

public:

    virtual void store(const JournalChecksumType &checksum, const size_t &size, const PathType &path) =0;

    virtual bool restore(const JournalChecksumType &checksum, const size_t &size, const PathType &path) = 0;

    virtual void update(const JournalChecksumType &checksum, const size_t &size, const PathType &path)=0;

    //syncs whole repository to corresponding repository from other node
    virtual void sync(const NodeIdType &nodeID)=0;

    virtual fs::path getResourcePath(const ResourceId &resourceId) =0;

    virtual bool hasResource(const ResourceId &resourceId) =0;

    IStorage(IRepository *r) {
        this->repository = r;
    }

};


#endif //BASYCO_ISTORAGE_H
