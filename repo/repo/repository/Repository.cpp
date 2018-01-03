//
// Created by stilgar on 17.10.17.
//

#include "Repository.h"

const IRepository::RepoIdType &Repository::getRepositoryId() const {
    return repositoryId;
}


JournalPtr &Repository::getJournal() {
    return journal;
}

void Repository::setJournal(const JournalPtr &journal) {
    Repository::journal = journal;
}

Repository::Repository(const RepoIdType &repositoryId) : repositoryId(repositoryId),
                                                         storage(std::make_shared<InternalStorage>(
                                                                 static_cast<IRepository *>(this))) {}

const std::shared_ptr<IStorage> &Repository::getStorage() const {
    return storage;
}

void Repository::restoreAll() {
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
        } else {
            //set modification date to correct value

            fs::last_write_time(i.getPath(), std::chrono::system_clock::from_time_t(i.getModificationTime()));
            fs::permissions(i.getPath(), i.getPermissions());
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

void Repository::buildFileMap() {
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

void Repository::persist(fs::path path) {
    journal->append(JournalMethod::ADDED, path);
    ///@todo take values from journal, or, even better, replay journal state during commit and do the store in that
    storage->store(calculateSha1OfFile(path), fs::file_size(path), path);
}
