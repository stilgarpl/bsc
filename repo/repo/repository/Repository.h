#include <utility>

//
// Created by stilgar on 17.10.17.
//

#ifndef BASYCO_REPOSITORY_H
#define BASYCO_REPOSITORY_H


#include <repo/journal/SimpleJournal.h>
#include <p2p/log/Logger.h>
#include <repo/repository/storage/IStorage.h>
#include <repo/repository/storage/InternalStorage.h>
#include <p2p/utils/crypto.h>
#include <repo/repository/transformer/IPathTransformer.h>
#include <repo/repository/transformer/PathTransformer.h>
#include "IRepository.h"

class Repository : public IRepository {
public:

    class RepoFileMap {
    private:
        class Attributes {
            fs::perms permissions;
            uintmax_t size = 0;
            std::time_t modificationTime;
            ResourceId checksum; //checksum of the file.
            bool directory = false;
            ResourceId resourceId;
        public:
            fs::perms getPermissions() const;

            uintmax_t getSize() const;

            time_t getModificationTime() const;

            const ResourceId &getChecksum() const;

            const ResourceId &getResourceId() const;

            Attributes() = default;

            Attributes(const JournalStateData &data);

            bool isDirectory() const;

            FileData toFileData() {
                return FileData(checksum, permissions, size, modificationTime, directory);
            }

            FileData toFileData(const fs::path &path) {
                return FileData(path, checksum, permissions, size, modificationTime, directory);
            }

        };

        class DeleteInfo {
        private:
            bool deleted = false;
            std::time_t deletionTime = 0;

        public:
            bool isDeleted() const;

            void setDeleted(bool deleted);

            time_t getDeletionTime() const;

            void setDeletionTime(time_t deletionTime);
        };
    private:
        std::map<fs::path, std::optional<Attributes>> attributesMap;
        std::map<fs::path, DeleteInfo> deleteMap;
        JournalPtr &journal;
        std::shared_ptr<IPathTransformer> &pathTransformer;
        decltype(journal->getChecksum()) mapChecksum;
    private:
        void prepareMap();
    public:
        auto operator[](const fs::path &path) -> decltype(attributesMap[fs::current_path()]);

        auto getSize(fs::path path);

        decltype(attributesMap) subMap(const fs::path root);

    public:

    public:

        auto begin() -> decltype(attributesMap.begin());

        auto end() -> decltype(attributesMap.end());

        std::time_t getDeletionTime(fs::path path) {
            return deleteMap[path].getDeletionTime();
        }

        auto isDeleted(fs::path path) {
            return deleteMap[path].isDeleted();
        }

        RepoFileMap(JournalPtr &journal, std::shared_ptr<IPathTransformer> &pathTransformer);
    };


private:
    JournalPtr journal = std::make_shared<SimpleJournal>();
    RepoIdType repositoryId;
    std::shared_ptr<IStorage> storage;
    std::shared_ptr<IPathTransformer> pathTransformer = std::make_shared<PathTransformer>();


    RepoFileMap _repoFileMap = RepoFileMap(journal, pathTransformer);

protected:

    //@todo move this function inside RepoFileMap. Store more data in RepoFileMap - it should know modification date of each path and type (dir/file)
    RepoFileMap &getFileMap() {
        return _repoFileMap;
    }

public:
    const RepoIdType &getRepositoryId() const override;

    //@todo hide this perhaps?
    JournalPtr &getJournal();

    const std::shared_ptr<IStorage> &getStorage() const;

    void setJournal(const JournalPtr &journal);

//    fs::path getStoragePath(const IStorage::ResourceId &resourceId) {
//        return storage->getResourcePath(resourceId);
//    };



    void downloadStorage();

    void restoreAll();

    void commit();

public:


    void persist(fs::path path);

    void forget(fs::path path);

    void remove(fs::path path);

    void ignore(fs::path path);

    void restoreAttributes(fs::path path);

    //update one file from the repository
    void update(fs::path path);

    //update everything
    void update();

    explicit Repository(const RepoIdType &repositoryId);

    void trash(fs::path path);
};

typedef std::shared_ptr<Repository> RepositoryPtr;

#endif //BASYCO_REPOSITORY_H
