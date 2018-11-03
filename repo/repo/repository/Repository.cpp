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
    //@todo _repoFileMap should have just access to Repository's journal.
//    _repoFileMap.setJournal(journal);
}

Repository::Repository(const RepoIdType &repositoryId) : repositoryId(repositoryId),
                                                         storage(std::make_shared<InternalStorage>(
                                                                 static_cast<IRepository *>(this))) {}

const std::shared_ptr<IStorage> &Repository::getStorage() const {
    return storage;
}

void Repository::restoreAll() {

    auto& fileMap = getFileMap();
    for (auto &&[path, value] :fileMap) {
        if (value) {
            LOGGER("restoring path " + path)
            storage->restore(value->getResourceId(), path);
            //@todo restore attributes
        } else {
            LOGGER("resAll: no value")
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
//@todo check if file is in fileMap, if it is, then method is *_EDITED
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
    for (const auto &[path, value] : fileMap) {
        LOGGER("file " + path + " => " + (value ? value->getResourceId() : std::string(" [X] ")));
        //@todo change this bool to actual transfer management

        //check for resources.
        if (value) {
            if (!storage->hasResource(value->getResourceId())) {
                hasResources &= storage->acquireResource(value->getResourceId());
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
    //only update if the file is in the repository
    auto &value = fileMap[path];
    if (value) {
        if (fs::exists(path)) {
            //@todo use some kind of FileUtil to get this
            auto currentFileTime = std::chrono::system_clock::to_time_t(fs::last_write_time(path));
            if (currentFileTime < value->getModificationTime()) {
                storage->restore(value->getResourceId(), path);
                //@todo restore attributes
            } else {
                if (currentFileTime == value->getModificationTime()) {

                } else {
                    //@todo pull file into repository? persist()?
                }
            }
        } else {
            storage->restore(value->getResourceId(), path);
            //@todo restore attributes
        }
    } else {
        //exception or sth?
        ;
    };


}

void Repository::update() {

    for (const auto &i : getFileMap()) {
        update(i.first);
    }

}

void Repository::RepoFileMap::prepareMap() {
//    LOGGER("prepare map jch:" + journal->getChecksum() + " mck " + mapChecksum )

    if (mapChecksum != journal->getChecksum()) {
        LOGGER("checksum different, recreate file map")
        attributesMap.clear();
        journal->clearFunc();
        journal->setFunc(JournalMethod::ADDED_FILE, [&](auto &i) {
            attributesMap[i.getPath()] = Attributes();
            attributesMap[i.getPath()]->setResourceId(
                    IStorage::getResourceId(i.getChecksum(), i.getSize()));//i.getChecksum();
            //@todo do one func to set them all from a file or journal data entry
            attributesMap[i.getPath()]->setPermissions(i.getPermissions());
            attributesMap[i.getPath()]->setSize(i.getSize());
            attributesMap[i.getPath()]->setModificationTime(i.getModificationTime());
            attributesMap[i.getPath()]->setChecksum(i.getChecksum());
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::MODIFIED_FILE, [&](auto &i) {
            attributesMap[i.getPath()] = Attributes();
            attributesMap[i.getPath()]->setResourceId(
                    IStorage::getResourceId(i.getChecksum(), i.getSize()));//i.getChecksum();
            attributesMap[i.getPath()]->setPermissions(i.getPermissions());
            attributesMap[i.getPath()]->setSize(i.getSize());
            attributesMap[i.getPath()]->setModificationTime(i.getModificationTime());
            attributesMap[i.getPath()]->setChecksum(i.getChecksum());
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        //@todo moved file should have two parameters - from to. or, just remove MOVED and use DELETED/ADDED
        journal->setFunc(JournalMethod::MOVED_FILE, [&](auto &i) {
            attributesMap[i.getPath()] = Attributes();
            attributesMap[i.getPath()]->setResourceId(
                    IStorage::getResourceId(i.getChecksum(), i.getSize()));//i.getChecksum();
            attributesMap[i.getPath()]->setPermissions(i.getPermissions());
            attributesMap[i.getPath()]->setSize(i.getSize());
            attributesMap[i.getPath()]->setModificationTime(i.getModificationTime());
            attributesMap[i.getPath()]->setChecksum(i.getChecksum());
            LOGGER(i.getChecksum() + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::DELETED_FILE, [&](auto &i) {
            attributesMap[i.getPath()] = std::nullopt;
//            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        //@todo set attributes for directories as well
        journal->replay();
        mapChecksum = journal->getChecksum();
    }
}

auto Repository::RepoFileMap::operator[](const PathType &path) -> decltype(attributesMap[0]) {
    //@todo future optimization: maybe this shouldn't be called every time? it has an if, but still...
    prepareMap();
    return attributesMap[path];
}

void Repository::RepoFileMap::clear() {
    attributesMap.clear();
    mapChecksum = "";
}

const std::string Repository::RepoFileMap::getJournalChecksum() const {
    return journal->getChecksum();
}

auto Repository::RepoFileMap::begin() -> decltype(attributesMap.begin()) {
    prepareMap();
    return attributesMap.begin();
}

auto Repository::RepoFileMap::end() -> decltype(attributesMap.end()) {
    return attributesMap.end();
}

Repository::RepoFileMap::RepoFileMap(JournalPtr &journal) : journal(journal) {}

void Repository::RepoFileMap::setJournal(const JournalPtr &journal) {
    RepoFileMap::journal = journal;
}

std::filesystem::perms Repository::RepoFileMap::Attributes::getPermissions() const {
    return permissions;
}

void Repository::RepoFileMap::Attributes::setPermissions(std::filesystem::perms permissions) {
    Attributes::permissions = permissions;
}

uintmax_t Repository::RepoFileMap::Attributes::getSize() const {
    return size;
}

void Repository::RepoFileMap::Attributes::setSize(uintmax_t size) {
    Attributes::size = size;
}

time_t Repository::RepoFileMap::Attributes::getModificationTime() const {
    return modificationTime;
}

void Repository::RepoFileMap::Attributes::setModificationTime(time_t modificationTime) {
    Attributes::modificationTime = modificationTime;
}

const ResourceId &Repository::RepoFileMap::Attributes::getChecksum() const {
    return checksum;
}

void Repository::RepoFileMap::Attributes::setChecksum(const ResourceId &checksum) {
    Attributes::checksum = checksum;
}

const ResourceId &Repository::RepoFileMap::Attributes::getResourceId() const {
    return resourceId;
}

void Repository::RepoFileMap::Attributes::setResourceId(const ResourceId &resourceId) {
    Attributes::resourceId = resourceId;
}
