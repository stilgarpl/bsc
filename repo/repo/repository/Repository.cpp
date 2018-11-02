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

    auto& fileMap = getFileMap();
    for (auto &&[path, resourceId] :fileMap) {
        if (resourceId) {
            storage->restore(*resourceId, path);
        }
    }
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

void Repository::persist(fs::path path) {

    if (path.is_relative()) {
        path = fs::canonical(fs::current_path() / path);
    }

    //@todo take values from journal, or, even better, replay journal state during commit and do the store in that
    if (!fs::is_directory(path)) {
        journal->append(JournalMethod::ADDED_FILE, path);

    } else {
        journal->append(JournalMethod::ADDED_DIRECTORY, path);
    }
}

void Repository::downloadStorage() {

    auto &fileMap = getFileMap();
    bool hasResources = true;
    for (const auto &[path, resourceId] : fileMap) {
        LOGGER("file " + path + " => " + (resourceId ? *resourceId : std::string(" [X] ")));
        //@todo change this bool to actual transfer management

        //check for resources.
        if (resourceId) {
            if (!storage->hasResource(*resourceId)) {
                hasResources &= storage->acquireResource(*resourceId);
            }
        }


    }
//
//    //@todo restore should not be in downloadStorage! move it to restoreAll
//    for (const auto &[path, resourceId] : fileMap) {
//        if (hasResources) {
//            //@todo add transform
//            storage->restore(*resourceId, path);
//        } else {
//            LOGGER("NO RESOURCES AVAILKABVLE")
//        }
//    }




}

void Repository::update(fs::path path) {

    auto &fileMap = getFileMap();


}
