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

    void setJournal(const JournalPtr &journal);

    fs::path getStoragePath(const IStorage::ResourceId &resourceId) {
        return storage->getResourcePath(resourceId);
    };


    void buildFileMap() {
        journal->setFunc(JournalMethod::ADDED, [&](auto &i) {
            fileMap[i.getPath()] = IStorage::getResourceId(i.getChecksum(), i.getSize());//i.getChecksum();
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::MOVED, [&](auto &i) {
            fileMap[i.getPath()] = IStorage::getResourceId(i.getChecksum(), i.getSize());
            LOGGER(i.getChecksum() + " ::: " + i.getPath());
        });


        journal->replay();
    }

    void restoreAll() {
        journal->setFunc(JournalMethod::ADDED, [&](auto &i) {
            ///@todo path transform
            bool ret = storage->restore(i.getChecksum(), i.getSize(), i.getPath());
            if (!ret) {
                //restore failed.
                auto resourceId = storage->getResourceId(i.getChecksum(), i.getSize());
                //check if the resource is in storage
                if (!storage->hasResource(resourceId)) {
                    //download from another repo
                    ///@todo download
                } else {
                    //weird, maybe no space left?
                    ///@todo error handling
                }
            }
//            fileMap[i.getPath()] = i.getChecksum();
//            LOGGER(i.getChecksum() + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::MOVED, [&](auto &i) {
//            fileMap[i.getPath()] = i.getChecksum();
//            LOGGER(i.getChecksum() + " ::: " + i.getPath());
        });


        journal->replay();
    }

public:


    void persist(fs::path path) {
        journal->append(JournalMethod::ADDED, path);
        ///@todo take values from journal, or, even better, replay journal state during commit and do the store in that
        storage->store(calculateSha1OfFile(path), fs::file_size(path), path);
    }

    Repository(const RepoIdType &repositoryId);

};

typedef std::shared_ptr<Repository> RepositoryPtr;

#endif //BASYCO_REPOSITORY_H
