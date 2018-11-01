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
#include "IRepository.h"

class Repository : public IRepository {
public:

    class RepoFileMap {

    private:
        //@todo maybe some class instead of just PathType?
        std::map<PathType, std::optional<ResourceId>> fileMap;
        std::string journalChecksum;
    public:
        std::optional<ResourceId> &operator[](const PathType &path) {
            return fileMap[path];
        }

        void clear() {
            fileMap.clear();
        }

    public:
        const std::string &getJournalChecksum() const {
            return journalChecksum;
        }

        void setJournalChecksum(const std::string &journalChecksum) {
            RepoFileMap::journalChecksum = journalChecksum;
        }

        auto begin() {
            return fileMap.begin();
        }

        auto end() {
            return fileMap.end();
        }

    };
private:
    JournalPtr journal = std::make_shared<SimpleJournal>();
    RepoIdType repositoryId;
    std::shared_ptr<IStorage> storage;


    RepoFileMap _repoFileMap;

protected:

    RepoFileMap &getFileMap() {
        if (journal->getChecksum() != _repoFileMap.getJournalChecksum()) {
            _repoFileMap.clear();
            journal->clearFunc();
            journal->setFunc(JournalMethod::ADDED_FILE, [&](auto &i) {
                _repoFileMap[i.getPath()] = IStorage::getResourceId(i.getChecksum(), i.getSize());//i.getChecksum();
                LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
            });

            journal->setFunc(JournalMethod::MODIFIED_FILE, [&](auto &i) {
                _repoFileMap[i.getPath()] = IStorage::getResourceId(i.getChecksum(), i.getSize());//i.getChecksum();
                LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
            });

            //@todo moved file should have two parameters - from to. or, just remove MOVED and use DELETED/ADDED
            journal->setFunc(JournalMethod::MOVED_FILE, [&](auto &i) {
                _repoFileMap[i.getPath()] = IStorage::getResourceId(i.getChecksum(), i.getSize());
                LOGGER(i.getChecksum() + " ::: " + i.getPath());
            });

            journal->setFunc(JournalMethod::DELETED_FILE, [&](auto &i) {
                _repoFileMap[i.getPath()] = std::nullopt;
                LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
            });

            journal->replay();
            _repoFileMap.setJournalChecksum(journal->getChecksum());
        }
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

    explicit Repository(const RepoIdType &repositoryId);

};

typedef std::shared_ptr<Repository> RepositoryPtr;

#endif //BASYCO_REPOSITORY_H
