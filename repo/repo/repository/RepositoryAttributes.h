//
// Created by stilgar on 30.08.2019.
//

#ifndef BSC_REPOSITORYATTRIBUTES_H
#define BSC_REPOSITORYATTRIBUTES_H

#include <filesystem>
#include <repo/journal/JournalTypes.h>
#include <repo/repository/storage/IStorage.h>
#include <repo/journal/JournalState.h>

namespace bsc {
    namespace fs = std::filesystem;

    class RepositoryAttributes {
        fs::perms permissions = fs::perms::none;
        uintmax_t size = 0;
        fs::file_time_type modificationTime = fs::file_time_type::min();
        ChecksumId checksum; //checksum of the file.
        bool directory = false;
        IStorage::ResourceId resourceId;
    public:
        fs::perms getPermissions() const;

        uintmax_t getSize() const;

        fs::file_time_type getModificationTime() const;

        const ChecksumId& getChecksum() const;

        const IStorage::ResourceId& getResourceId() const;

        RepositoryAttributes() = default;

        explicit RepositoryAttributes(const JournalStateData& data);

        bool isDirectory() const;

        bsc::FileData toFileData(const fs::path& path) {
            return bsc::FileData(path, checksum, permissions, size, modificationTime, directory);
        }

    };
}

#endif //BSC_REPOSITORYATTRIBUTES_H
