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
    std::time_t modificationTime;
    bool isDirectory = false;

public:
    bool isIsDirectory() const;

public:
    const std::filesystem::path &getCanonicalPath() const;

    const std::string &getSha256hash() const;

    std::filesystem::perms getPermissions() const;

    const uintmax_t &getSize() const;

    const std::time_t &getModificationTime() const;

    explicit FileData(const fs::path &path) {
        if (fs::exists(path) && !fs::is_directory(path)) {
            canonicalPath = fs::canonical(path);
            size = fs::file_size(fs::path(path));
            modificationTime = std::chrono::system_clock::to_time_t(fs::last_write_time(path));
            permissions = fs::status(path).permissions();
            isDirectory = false;
            CryptoPP::SHA256 hash;
            std::string digest;
            CryptoPP::FileSource f(path.c_str(), true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(digest))));
            sha256hash = std::move(digest);
        } else if (fs::is_directory(path)) {
            isDirectory = true;
            canonicalPath = fs::canonical(path);
            modificationTime = std::chrono::system_clock::to_time_t(fs::last_write_time(path));
            permissions = fs::status(path).permissions();
        } else {
            //@todo throw?
        }
    }
};


#endif //BASYCO_FILEDATA_H
