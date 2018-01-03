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

private:
    JournalPtr journal = std::make_shared<SimpleJournal>();
    RepoIdType repositoryId;
    std::shared_ptr<IStorage> storage;


    ///@todo maybe some class instead of just PathType?
    std::map<PathType, ResourceId> fileMap;

public:
    const RepoIdType &getRepositoryId() const;

    JournalPtr &getJournal();

    const std::shared_ptr<IStorage> &getStorage() const;

    void setJournal(const JournalPtr &journal);

//    fs::path getStoragePath(const IStorage::ResourceId &resourceId) {
//        return storage->getResourcePath(resourceId);
//    };


    void buildFileMap();

    void restoreAll();

public:


    void persist(fs::path path);

    Repository(const RepoIdType &repositoryId);

};

typedef std::shared_ptr<Repository> RepositoryPtr;

#endif //BASYCO_REPOSITORY_H
