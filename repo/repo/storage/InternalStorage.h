//
// Created by stilgar on 10.12.17.
//

#ifndef BASYCO_INTERNALSTORAGE_H
#define BASYCO_INTERNALSTORAGE_H


#include "IStorage.h"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

class InternalStorage : public IStorage {

private:
    std::string storageId;
    fs::path storagePath = "/tmp/internal/";
public:
    void store(const JournalChecksumType &checksum, const size_t &size, const PathType &sourcePath) override;

    bool restore(const JournalChecksumType &checksum, const size_t &size, const PathType &destinationPath) override;

    void update(const JournalChecksumType &checksum, const size_t &size, const PathType &sourcePath) override;

    void sync(const NodeIdType &nodeID) override;

    void initStorage();


    InternalStorage(const std::string &storageId);
};


#endif //BASYCO_INTERNALSTORAGE_H
