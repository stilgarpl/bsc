//
// Created by stilgar on 30.08.2019.
//

#include "RepositoryAttributes.h"



fs::perms RepositoryAttributes::getPermissions() const {
    return permissions;
}

uintmax_t RepositoryAttributes::getSize() const {
    return size;
}

fs::file_time_type RepositoryAttributes::getModificationTime() const {
    return modificationTime;
}

const ChecksumId &RepositoryAttributes::getChecksum() const {
    return checksum;
}

const IStorage::ResourceId &RepositoryAttributes::getResourceId() const {
    return resourceId;
}

RepositoryAttributes::RepositoryAttributes(const JournalStateData &data) {
    size = data.getSize();
    permissions = data.getPermissions();
    modificationTime = data.getModificationTime();
    checksum = data.getChecksum();
    resourceId = IStorage::getResourceId(data.getChecksum(), data.getSize());
    directory = data.isDirectory();
}

bool RepositoryAttributes::isDirectory() const {
    return directory;
}
