//
// Created by stilgar on 10.12.17.
//

#ifndef BASYCO_ISTORAGE_H
#define BASYCO_ISTORAGE_H


#include <repo/journal/JournalTypes.h>
#include <p2p/network/node/NodeInfo.h>

class IStorage {


public:

    virtual void store(const JournalChecksumType &checksum, const size_t &size, const PathType &path) =0;

    virtual bool restore(const JournalChecksumType &checksum, const size_t &size, const PathType &path) = 0;

    virtual void update(const JournalChecksumType &checksum, const size_t &size, const PathType &path)=0;

    //syncs whole repository to corresponding repository from other node
    virtual void sync(const NodeIdType &nodeID)=0;

};


#endif //BASYCO_ISTORAGE_H
