//
// Created by stilgar on 04.11.18.
//

#ifndef BASYCO_FILEDATA_H
#define BASYCO_FILEDATA_H


#include <filesystem>
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
    fs::file_time_type modificationTime = fs::file_time_type::min();
    bool isDirectory = false;

public:
    bool isIsDirectory() const;

public:
    const std::filesystem::path &getCanonicalPath() const;

    const std::string &getSha256hash() const;

    std::filesystem::perms getPermissions() const;

    const uintmax_t &getSize() const;

    const fs::file_time_type &getModificationTime() const;

    explicit FileData(const fs::path &path);

    FileData(std::filesystem::path canonicalPath, std::string sha256hash,
             std::filesystem::perms permissions, uintmax_t size, fs::file_time_type modificationTime, bool isDirectory);

    FileData(std::string sha256hash, std::filesystem::perms permissions, uintmax_t size,
             fs::file_time_type modificationTime,
             bool isDirectory);
};


#endif //BASYCO_FILEDATA_H
