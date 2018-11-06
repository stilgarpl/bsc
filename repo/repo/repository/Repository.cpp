//
// Created by stilgar on 17.10.17.
//

#include <repo/repository/transformer/rules/TmpRule.h>
#include <repo/repository/transformer/rules/HomeDirRule.h>
#include "Repository.h"

using namespace std::chrono_literals;
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
                                                                 static_cast<IRepository *>(this))) {
    pathTransformer->addRule(std::make_shared<TmpRule>());
    pathTransformer->addRule(std::make_shared<HomeDirRule>());
}

const std::shared_ptr<IStorage> &Repository::getStorage() const {
    return storage;
}

void Repository::restoreAll() {

    auto& fileMap = getFileMap();
    for (auto &&[path, value] :fileMap) {
        if (value) {
            LOGGER("restoring path " + path.string())
            if (value->isDirectory()) {
                fs::create_directories(path);
            } else {
                storage->restore(value->getResourceId(), path);
            }
            restoreAttributes(path);
        } else {
            LOGGER("resAll: no value")
        }
    }
}

void Repository::commit() {
    journal->clearFunc();
    journal->setFunc(JournalMethod::ADDED_FILE, [&](auto &i) {
        storage->store(calculateSha1OfFile(pathTransformer->transformFromJournalFormat(i.getPath())),
                       fs::file_size(pathTransformer->transformFromJournalFormat(i.getPath())),
                       pathTransformer->transformFromJournalFormat(i.getPath()));
        LOGGER("commit: added file " + pathTransformer->transformFromJournalFormat(i.getPath()).string())
    });

    journal->setFunc(JournalMethod::MOVED_FILE, [&](auto &i) {
    });

    journal->setFunc(JournalMethod::ADDED_DIRECTORY, [&](auto &i) {
        //nothing to store, but... files from the directory should be added.
        LOGGER("commit: added dir " + i.getPath() + " tt: " +
               pathTransformer->transformFromJournalFormat(i.getPath()).string())
        fs::path dirPath = pathTransformer->transformFromJournalFormat(i.getPath());
        for (const auto &item : fs::directory_iterator(dirPath)) {
            fs::path path = fs::canonical(item.path());
            if (fs::is_directory(item)) {
                journal->append(ADDED_DIRECTORY, pathTransformer->transformToJournalFormat(path),
                                FileData(item.path()));
            } else {
                journal->append(ADDED_FILE, pathTransformer->transformToJournalFormat(path),
                                FileData(item.path()));
            }
        }
    });

    journal->setFunc(JournalMethod::MODIFIED_DIRECTORY, [&](auto &i) {
        fs::path dirPath = pathTransformer->transformFromJournalFormat(i.getPath());

        auto &fileMap = getFileMap();
        for (const auto &item : fs::directory_iterator(dirPath)) {
            fs::path path = fs::canonical(item.path());
            auto &attr = fileMap[path];
            if (fs::exists(fs::canonical(item.path()))) {
                if (!attr) {
                    if (fs::is_directory(path)) {
                        journal->append(ADDED_DIRECTORY, pathTransformer->transformToJournalFormat(path),
                                        FileData(item.path()));
                    } else {
                        journal->append(ADDED_FILE, pathTransformer->transformToJournalFormat(path),
                                        FileData(item.path()));
                    }
                }
            } else {
                if (attr) {
                    if (attr->isDirectory()) {
                        journal->append(DELETED_DIRECTORY, pathTransformer->transformToJournalFormat(path),
                                        FileData(item.path()));
                    } else {
                        journal->append(DELETED_FILE, pathTransformer->transformToJournalFormat(path),
                                        FileData(item.path()));
                    }
                }

            }
        }
    });

    journal->setFunc(JournalMethod::DELETED_DIRECTORY, [&](auto &i) {
        //@todo delete everything recursively
        auto subMap = getFileMap().subMap(pathTransformer->transformFromJournalFormat(i.getPath()));
        for (const auto &[subPath, value] : subMap) {
            //this will not set the isDirectory flag, but I don't think it's important.
            // @todo maybe I should split METHOD into two: METHOD and TARGET (DELETED, DIRECTORY)
            if (value->isDirectory()) {
                journal->append(JournalMethod::DELETED_DIRECTORY, pathTransformer->transformToJournalFormat(subPath),
                                FileData(subPath));
            } else {
                journal->append(JournalMethod::DELETED_FILE, pathTransformer->transformToJournalFormat(subPath),
                                FileData(subPath));
            }
        }
    });


    journal->replayCurrentState();
    journal->commitState();
}

void Repository::persist(fs::path path) {

    auto &fileMap = getFileMap();
    if (path.is_relative()) {
        path = fs::canonical(fs::current_path() / path);
    }

    auto &attr = fileMap[path];
    if (attr) {
        //file exists in map! update mode

        //@todo check if file was actually changed.
        if (!attr->isDirectory()) {
            journal->append(JournalMethod::MODIFIED_FILE, pathTransformer->transformToJournalFormat(path),
                            FileData(path));

        } else {
            journal->append(JournalMethod::MODIFIED_DIRECTORY, pathTransformer->transformToJournalFormat(path),
                            FileData(path));
        }
    } else {
        if (!fs::is_directory(path)) {
            journal->append(JournalMethod::ADDED_FILE, pathTransformer->transformToJournalFormat(path),
                            FileData(path));

        } else {
            journal->append(JournalMethod::ADDED_DIRECTORY, pathTransformer->transformToJournalFormat(path),
                            FileData(path));
        }
    }
}

void Repository::downloadStorage() {

    auto &fileMap = getFileMap();
    bool hasResources = true;
    for (const auto &[path, value] : fileMap) {
        LOGGER("file " + path.string() + " => " + (value ? value->getResourceId() : std::string(" [X] ")));
        //@todo change this bool to actual transfer management

        //check for resources.
        if (value) {
            if (!storage->hasResource(value->getResourceId())) {
                hasResources &= storage->acquireResource(value->getResourceId());
            }
        }


    }

}

void Repository::update(fs::path path) {

    auto &fileMap = getFileMap();
    //only update if the file is in the repository
    auto &value = fileMap[path];
    auto &attr = fileMap[path];
    if (fs::exists(path)) {
        if (attr) {
            auto currentFileTime = std::chrono::system_clock::to_time_t(fs::last_write_time(path));
            LOGGER("current time " + std::to_string(currentFileTime) + " lwt " +
                   std::to_string(value->getModificationTime()))
            if (currentFileTime < value->getModificationTime()) {
                LOGGER("restoring..." + path.string())
                storage->restore(value->getResourceId(), path);
                restoreAttributes(path);
            } else {
                if (currentFileTime == value->getModificationTime()) {
                    //@todo verify other things, like size() maybe
                    LOGGER("leaving alone " + path.string())
                } else {
                    LOGGER("persisting... " + path.string())
                    persist(path); //this should add file as modified.
                }
            }
        } else {
            //no
        }
    } else {
        //file was removed

        if (attr) {
            //file is in file map, but not on filesystem, removing
            LOGGER("forgetting " + path.string());
            forget(path);
        }
    }
//    if (value) {
//        if (fs::exists(path)) {
//            //@tod if fs::is_directory(path), iterate over files and directories and persist new ones.
//            //@todo use some kind of FileUtil to get this
//            auto currentFileTime = std::chrono::system_clock::to_time_t(fs::last_write_time(path));
//            LOGGER("current time " + std::to_string(currentFileTime) + " lwt " +
//                   std::to_string(value->getModificationTime()))
//            if (currentFileTime < value->getModificationTime()) {
//                LOGGER("restoring...")
//                storage->restore(value->getResourceId(), path);
//                restoreAttributes(path);
//            } else {
//                if (currentFileTime == value->getModificationTime()) {
//
//                } else {
//                    //@todo pull file into repository? persist()?
//                    persist(path); //this should add file as modified.
//                }
//            }
//        } else {
//            //@todo reform those ifs to combine two restore paths
//            storage->restore(value->getResourceId(), path);
//            restoreAttributes(path);
//        }
//    } else {
//        //exception or sth?
//        ;
//    };


}

void Repository::update() {

    for (const auto &i : getFileMap()) {
        update(i.first);
    }

}

void Repository::restoreAttributes(fs::path path) {

    auto &fileMap = getFileMap();

    if (fs::exists(path)) {
        auto &attributes = fileMap[path];
        fs::permissions(path, attributes->getPermissions());
        fs::last_write_time(path, std::chrono::system_clock::from_time_t(attributes->getModificationTime()));
    }

}

void Repository::forget(fs::path path) {

    auto &fileMap = getFileMap();
    if (path.is_relative()) {
        //@todo not so sure about current path, i have to make sure this is always set to the right value
        path = fs::canonical(fs::current_path() / path);
    }
    if (fileMap[path]) {
        if (!fs::is_directory(path)) {
            journal->append(JournalMethod::DELETED_FILE, pathTransformer->transformToJournalFormat(path),
                            FileData(path));

        } else {
            journal->append(JournalMethod::DELETED_DIRECTORY, pathTransformer->transformToJournalFormat(path),
                            FileData(path));
            //@todo delete everything recursively ... or maybe do it in replayCurrentState?

        }
    } else {
        //nothing to forget!
    }
}

void Repository::ignore(fs::path path) {
    auto &fileMap = getFileMap();
    if (path.is_relative()) {
        //@todo not so sure about current path, i have to make sure this is always set to the right value
        path = fs::canonical(fs::current_path() / path);
    }
    if (!fs::is_directory(path)) {
        journal->append(JournalMethod::IGNORED_FILE, pathTransformer->transformToJournalFormat(path),
                        FileData(path));

    } else {
        journal->append(JournalMethod::IGNORED_DIRECTORY, pathTransformer->transformToJournalFormat(path),
                        FileData(path));
        //@todo ignore everything recursively
    }
}

void Repository::RepoFileMap::prepareMap() {
    LOGGER("prepare map jch:" + journal->getChecksum() + " mck " + mapChecksum)

    if (mapChecksum != journal->getChecksum()) {
        LOGGER("checksum different, recreate file map")
        attributesMap.clear();
        journal->clearFunc();
        journal->setFunc(JournalMethod::ADDED_FILE, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = Attributes(i);
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::MODIFIED_FILE, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = Attributes(i);
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        //@todo moved file should have two parameters - from to. or, just remove MOVED and use DELETED/ADDED
        journal->setFunc(JournalMethod::MOVED_FILE, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = Attributes(i);
        });

        journal->setFunc(JournalMethod::DELETED_FILE, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = std::nullopt;
//            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::ADDED_DIRECTORY, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = Attributes(i);
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::MODIFIED_DIRECTORY, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = Attributes(i);
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        //@todo moved file should have two parameters - from to. or, just remove MOVED and use DELETED/ADDED
        journal->setFunc(JournalMethod::MOVED_DIRECTORY, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = Attributes(i);
        });

        journal->setFunc(JournalMethod::DELETED_DIRECTORY, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = std::nullopt;
//            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        //@todo set attributes for ignores.
        journal->replay();
        mapChecksum = journal->getChecksum();
    }
}

auto Repository::RepoFileMap::operator[](const fs::path &path) -> decltype(attributesMap[fs::current_path()]) {
    //@todo future optimization: maybe this shouldn't be called every time? it has an if, but still...
    prepareMap();
    return attributesMap[path];
}

auto Repository::RepoFileMap::begin() -> decltype(attributesMap.begin()) {
    prepareMap();
    return attributesMap.begin();
}

auto Repository::RepoFileMap::end() -> decltype(attributesMap.end()) {
    return attributesMap.end();
}

Repository::RepoFileMap::RepoFileMap(JournalPtr &journal, std::shared_ptr<IPathTransformer> &pathTransformer) : journal(
        journal), pathTransformer(pathTransformer) {}

auto Repository::RepoFileMap::getSize(fs::path path) {
    return attributesMap[path]->getSize();
}

decltype(Repository::RepoFileMap::attributesMap) Repository::RepoFileMap::subMap(const fs::path root) {
    decltype(Repository::RepoFileMap::attributesMap) result;
    for (const auto &[path, value] : attributesMap) {
        if (path.string().find(root.string()) != std::string::npos) {
            result[path] = value;
        }
    }
    return result;
}

fs::perms Repository::RepoFileMap::Attributes::getPermissions() const {
    return permissions;
}

uintmax_t Repository::RepoFileMap::Attributes::getSize() const {
    return size;
}

time_t Repository::RepoFileMap::Attributes::getModificationTime() const {
    return modificationTime;
}

const ResourceId &Repository::RepoFileMap::Attributes::getChecksum() const {
    return checksum;
}

const ResourceId &Repository::RepoFileMap::Attributes::getResourceId() const {
    return resourceId;
}

Repository::RepoFileMap::Attributes::Attributes(const JournalStateData &data) {
    size = data.getSize();
    permissions = data.getPermissions();
    modificationTime = data.getModificationTime();
    checksum = data.getChecksum();
    resourceId = IStorage::getResourceId(data.getChecksum(), data.getSize());
    directory = data.isDirectory();
}

bool Repository::RepoFileMap::Attributes::isDirectory() const {
    return directory;
}


