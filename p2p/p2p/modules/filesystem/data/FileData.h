//
// Created by stilgar on 04.11.18.
//

#ifndef BASYCO_FILEDATA_H
#define BASYCO_FILEDATA_H


#include <filesystem>
#include <bits/types/time_t.h>
#include <cryptopp/sha.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>


namespace fs = std::filesystem;

class FileData {
private:
    fs::path canonicalPath;
    std::string sha256hash;
    fs::perms permissions;
    uintmax_t size = 0;
    std::time_t modificationTime = 0;
    bool isDirectory = false;

public:
    bool isIsDirectory() const;

public:
    const std::filesystem::path &getCanonicalPath() const;

    const std::string &getSha256hash() const;

    std::filesystem::perms getPermissions() const;

    const uintmax_t &getSize() const;

    const std::time_t &getModificationTime() const;

    explicit FileData(const fs::path &path);

    FileData &setModificationTime(time_t modificationTime);
};


#endif //BASYCO_FILEDATA_H
