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
    std::map<PathType, JournalChecksumType> fileMap;

public:
    const RepoIdType &getRepositoryId() const;

    JournalPtr &getJournal();

    void setJournal(const JournalPtr &journal);


    void buildFileMap() {
        journal->setFunc(JournalMethod::ADDED, [&](auto &i) {
            fileMap[i.getPath()] = i.getChecksum();
            LOGGER(i.getChecksum() + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::MOVED, [&](auto &i) {
            fileMap[i.getPath()] = i.getChecksum();
            LOGGER(i.getChecksum() + " ::: " + i.getPath());
        });


        journal->replay();
    }

    void restoreAll() {
        journal->setFunc(JournalMethod::ADDED, [&](auto &i) {
            ///@todo path transform
            storage->restore(i.getChecksum(), i.getSize(), i.getPath());
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
