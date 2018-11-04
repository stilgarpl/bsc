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
