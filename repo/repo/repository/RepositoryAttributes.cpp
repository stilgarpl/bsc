//
// Created by stilgar on 30.08.2019.
//

#include "RepositoryAttributes.h"
namespace bsc {


    fs::perms RepositoryAttributes::getPermissions() const {
        return permissions;
    }

    uintmax_t RepositoryAttributes::getSize() const {
        return size;
    }

    fs::file_time_type RepositoryAttributes::getModificationTime() const {
        return modificationTime;
    }

    const ChecksumId& RepositoryAttributes::getChecksum() const {
        return checksum;
    }

    const IStorage::ResourceId& RepositoryAttributes::getResourceId() const {
        return resourceId;
    }

    RepositoryAttributes::RepositoryAttributes(const JournalStateData<JournalTarget::file>& data) {
        //@todo I'm not sure it it should be a constructor and not a conversion method. this way RepositoryAttributtes wouldn't have to know about JournalStateData
        size = data.getSize();
        permissions = data.getPermissions();
        modificationTime = data.getModificationTime();
        checksum = data.getResourceChecksum();
        //@todo replace getResourceId with resourceIdGenerator.
        resourceId = IStorage::getResourceId(data.getResourceChecksum(), data.getSize());
        directory = data.getTarget() == JournalTarget::directory; //@todo this is always false
    }

    RepositoryAttributes::RepositoryAttributes(const JournalStateData<JournalTarget::directory>& data) {
        //@todo I'm not sure it it should be a constructor and not a conversion method. this way RepositoryAttributtes wouldn't have to know about JournalStateData
        size = data.getSize();
        permissions = data.getPermissions();
        modificationTime = data.getModificationTime();
        checksum = data.getResourceChecksum();
        //@todo replace getResourceId with resourceIdGenerator.
        resourceId = IStorage::getResourceId(data.getResourceChecksum(), data.getSize());
        directory = data.getTarget() == JournalTarget::directory;//@todo this is always true
    }

    bool RepositoryAttributes::isDirectory() const {
        return directory;
    }
}