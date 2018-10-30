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
    journal->clearFunc();
    journal->setFunc(JournalMethod::ADDED_FILE, [&](auto &i) {
        //@todo path transform
        bool ret = storage->restore(i.getChecksum(), i.getSize(), i.getPath());
        if (!ret) {
            //restore failed.
            auto resourceId = storage->getResourceId(i.getChecksum(), i.getSize());
            //check if the resource is in storage
            if (!storage->hasResource(resourceId)) {
                //download from another repo
                //@todo download
            } else {
                //weird, maybe no space left?
                //@todo error handling
            }
        } else {
            //set modification date to correct value

            fs::last_write_time(i.getPath(), std::chrono::system_clock::from_time_t(i.getModificationTime()));
            fs::permissions(i.getPath(), i.getPermissions());
        }
//            fileMap[i.getPath()] = i.getChecksum();
//            LOGGER(i.getChecksum() + " ::: " + i.getPath());
    });

    journal->setFunc(JournalMethod::MOVED_FILE, [&](auto &i) {
//            fileMap[i.getPath()] = i.getChecksum();
//            LOGGER(i.getChecksum() + " ::: " + i.getPath());
    });

    journal->setFunc(JournalMethod::ADDED_DIRECTORY, [&](auto &i) {
        fs::create_directories(i.getPath());
    });


    journal->replay();
}

void Repository::commit() {
    journal->clearFunc();
    journal->setFunc(JournalMethod::ADDED_FILE, [&](auto &i) {
        storage->store(calculateSha1OfFile(i.getPath()), fs::file_size(i.getPath()), i.getPath());
        LOGGER("commit: added file " + i.getPath())
    });

    journal->setFunc(JournalMethod::MOVED_FILE, [&](auto &i) {
    });

    journal->setFunc(JournalMethod::ADDED_DIRECTORY, [&](auto &i) {
        //nothing to store, but... files from the directory should be added.
        LOGGER("commit: added dir " + i.getPath())
        for (const auto &item : fs::directory_iterator(i.getPath())) {
            if (fs::is_directory(item)) {
                //@todo we shouldn't process that... or we should and not use recursive iterator (disabled recursive for now)
                journal->append(ADDED_DIRECTORY, item.path());
            } else {
                //@todo make sure this is processed by ADDED_FILE func, even though we are modifying the container
                journal->append(ADDED_FILE, item.path());
            }
        }
    });


    journal->replayCurrentState();
    journal->commitState();
}

void Repository::buildFileMap() {
    journal->clearFunc();
    journal->setFunc(JournalMethod::ADDED_FILE, [&](auto &i) {
        fileMap[i.getPath()] = IStorage::getResourceId(i.getChecksum(), i.getSize());//i.getChecksum();
        LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
    });

    journal->setFunc(JournalMethod::MOVED_FILE, [&](auto &i) {
        fileMap[i.getPath()] = IStorage::getResourceId(i.getChecksum(), i.getSize());
        LOGGER(i.getChecksum() + " ::: " + i.getPath());
    });

    journal->replay();
}

void Repository::persist(fs::path path) {

    //@todo take values from journal, or, even better, replay journal state during commit and do the store in that
    if (!fs::is_directory(path)) {
        journal->append(JournalMethod::ADDED_FILE, path);

    } else {
        journal->append(JournalMethod::ADDED_DIRECTORY, path);
    }
}
