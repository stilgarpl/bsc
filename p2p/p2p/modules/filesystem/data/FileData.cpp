#include <utility>
#include <core/utils/crypto.h>

//
// Created by stilgar on 04.11.18.
//

#include "FileData.h"

const std::filesystem::path& FileData::getCanonicalPath() const {
    return canonicalPath;
}

const std::string& FileData::getSha256hash() const {
    return sha256hash;
}

std::filesystem::perms FileData::getPermissions() const {
    return permissions;
}

const uintmax_t& FileData::getSize() const {
    return size;
}

const fs::file_time_type& FileData::getModificationTime() const {
    return modificationTime;
}

bool FileData::isIsDirectory() const {
    return isDirectory;
}

FileData::FileData(const fs::path& path) {
    if (fs::exists(path)) {
        canonicalPath = fs::canonical(path);
        modificationTime = fs::last_write_time(path);
        permissions = fs::status(path).permissions();
        if (fs::is_directory(path)) {
            isDirectory = true;
            size = 0;
        } else {
            size = fs::file_size(fs::path(path));
            isDirectory = false;
            sha256hash = calculateSha1OfFile(path);
        }
    } else {
        //@todo throw? or just leave it empty? or setDirect an "empty flag? i think it should be left empty, it will work nicely with deleting files in JournalState
        canonicalPath = fs::weakly_canonical(path);
    }

}

FileData::FileData(std::filesystem::path canonicalPath, std::string sha256hash,
                   std::filesystem::perms permissions, uintmax_t size, fs::file_time_type modificationTime,
                   bool isDirectory)
        : canonicalPath(std::move(canonicalPath)), sha256hash(std::move(sha256hash)), permissions(permissions),
          size(size),
          modificationTime(modificationTime), isDirectory(isDirectory) {}

