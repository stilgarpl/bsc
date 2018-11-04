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

            ResourceId resourceId;
        public:
            fs::perms getPermissions() const;

            void setPermissions(std::filesystem::perms permissions);

            uintmax_t getSize() const;

            void setSize(uintmax_t size);

            time_t getModificationTime() const;

            void setModificationTime(time_t modificationTime);

            const ResourceId &getChecksum() const;

            void setChecksum(const ResourceId &checksum);

            const ResourceId &getResourceId() const;

            void setResourceId(const ResourceId &resourceId);


        };
    private:
        //@todo maybe some class instead of just PathType?
        // std::map<PathType, std::optional<ResourceId>> fileMap;
        std::map<PathType, std::optional<Attributes>> attributesMap;
        JournalPtr &journal;
        std::shared_ptr<IPathTransformer> &pathTransformer;
        decltype(journal->getChecksum()) mapChecksum;
    private:
        void prepareMap();
    public:
        auto operator[](const PathType &path) -> decltype(attributesMap[0]);

        auto getPermissions(fs::path path) {
            return attributesMap[path]->getPermissions();
        }

        void setJournal(const JournalPtr &journal);

        auto getSize(fs::path path) {
            return attributesMap[path]->getSize();
        }

        auto getChecksum(fs::path path) {
            return attributesMap[path]->getChecksum();
        }

        auto getModificationTime(fs::path path) {
            return attributesMap[path]->getModificationTime();
        }

        void clear();

    public:
    protected:
        const std::string getJournalChecksum() const;

    public:

        auto begin() -> decltype(attributesMap.begin());

        auto end() -> decltype(attributesMap.end());

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

    void restoreAttributes(fs::path path);

    //update one file from the repository
    void update(fs::path path);

    //update everything
    void update();

    explicit Repository(const RepoIdType &repositoryId);

};

typedef std::shared_ptr<Repository> RepositoryPtr;

#endif //BASYCO_REPOSITORY_H
