//
// Created by stilgar on 04.11.18.
//

#include <bits/types/time_t.h>
#include "FileData.h"

const std::filesystem::path &FileData::getCanonicalPath() const {
    return canonicalPath;
}

const std::string &FileData::getSha256hash() const {
    return sha256hash;
}

std::filesystem::perms FileData::getPermissions() const {
    return permissions;
}

const uintmax_t &FileData::getSize() const {
    return size;
}

const std::time_t &FileData::getModificationTime() const {
    return modificationTime;
}

bool FileData::isIsDirectory() const {
    return isDirectory;
}

FileData::FileData(const fs::path &path) {
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
    } else if (fs::exists(path) && fs::is_directory(path)) {
        isDirectory = true;
        canonicalPath = fs::canonical(path);
        modificationTime = std::chrono::system_clock::to_time_t(fs::last_write_time(path));
        permissions = fs::status(path).permissions();
        size = 0;
    } else {
        //@todo throw? or just leave it empty? or set an "empty flag? i think it should be left empty, it will work nicely with deleting files in JournalState
        canonicalPath = fs::weakly_canonical(path);
    }
}

FileData::FileData(const std::filesystem::path &canonicalPath, const std::string &sha256hash,
                   std::filesystem::perms permissions, uintmax_t size, time_t modificationTime, bool isDirectory)
        : canonicalPath(canonicalPath), sha256hash(sha256hash), permissions(permissions), size(size),
          modificationTime(modificationTime), isDirectory(isDirectory) {}

FileData::FileData(const std::string &sha256hash, std::filesystem::perms permissions, uintmax_t size,
                   time_t modificationTime, bool isDirectory) : sha256hash(sha256hash), permissions(permissions),
                                                                size(size), modificationTime(modificationTime),
                                                                isDirectory(isDirectory) {}
