#include <utility>

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

Repository::Repository(RepoIdType repositoryId) : strategyFactory(*this),
                                                  repositoryId(std::move(repositoryId)),
                                                  storage(std::make_shared<InternalStorage>(
                                                          static_cast<IRepository *>(this))),
                                                  deployPack(strategyFactory.createPack(
                                                          {.updatedInRepo = RepositoryAction::RESTORE,
                                                                  .updatedInFilesystem = RepositoryAction::NOP,
                                                                  .same = RepositoryAction::NOP,
                                                                  .newInRepo = RepositoryAction::RESTORE,
                                                                  .newInFilesystem = RepositoryAction::NOP,
                                                                  .deletedInRepo = RepositoryAction::TRASH,
                                                                  .deletedInFilesystem = RepositoryAction::RESTORE,
                                                          })),
                                                  localSyncPack(strategyFactory.createPack(
                                                          {.updatedInRepo = RepositoryAction::NOP,
                                                                  .updatedInFilesystem = RepositoryAction::UPDATE,
                                                                  .same = RepositoryAction::NOP,
                                                                  .newInRepo = RepositoryAction::NOP,
                                                                  .newInFilesystem = RepositoryAction::PERSIST,
                                                                  .deletedInRepo = RepositoryAction::NOP,
                                                                  .deletedInFilesystem = RepositoryAction::REMOVE,
                                                          })),
                                                  fullPack(strategyFactory.createPack(
                                                          {.updatedInRepo = RepositoryAction::RESTORE,
                                                                  .updatedInFilesystem = RepositoryAction::UPDATE,
                                                                  .same = RepositoryAction::NOP,
                                                                  .newInRepo = RepositoryAction::RESTORE,
                                                                  .newInFilesystem = RepositoryAction::PERSIST,
                                                                  .deletedInRepo = RepositoryAction::TRASH,
                                                                  .deletedInFilesystem = RepositoryAction::REMOVE,
                                                          })) {
    pathTransformer->addRule(std::make_shared<TmpRule>());
    pathTransformer->addRule(std::make_shared<HomeDirRule>());
}

const std::shared_ptr<IStorage> &Repository::getStorage() const {
    return storage;
}

void Repository::restoreAll() {

    //@todo this method kind of does the same as deploy() merge the two
    auto &fileMap = getFileMap();
    for (auto &&[path, value] :fileMap) {
        if (value) {
            LOGGER("restoring path " + path.string())
            if (value->isDirectory()) {
                fs::create_directories(path);
            } else {
                storage->restore(value->getResourceId(), path);
            }
            deployMap.markDeployed(path, Repository::DeployState::DEPLOYED);
            restoreAttributes(path);
        } else {
            LOGGER("resAll: no value")
        }
    }
}

void Repository::commit() {
    journal->clearFunc();
    journal->setFunc(JournalMethod::ADDED, JournalTarget::FILE, [&](auto &i) {
        storage->store(calculateSha1OfFile(pathTransformer->transformFromJournalFormat(i.getPath())),
                       fs::file_size(pathTransformer->transformFromJournalFormat(i.getPath())),
                       pathTransformer->transformFromJournalFormat(i.getPath()));
        LOGGER("commit: added file " + pathTransformer->transformFromJournalFormat(i.getPath()).string())
    });

    journal->setFunc(JournalMethod::MOVED, JournalTarget::FILE, [&](auto &i) {
    });

    journal->setFunc(JournalMethod::ADDED, JournalTarget::DIRECTORY, [&](auto &i) {
        //nothing to store, but... files from the directory should be added.
        LOGGER("commit: added dir " + i.getPath() + " tt: " +
               pathTransformer->transformFromJournalFormat(i.getPath()).string())
        fs::path dirPath = pathTransformer->transformFromJournalFormat(i.getPath());
        for (const auto &item : fs::directory_iterator(dirPath)) {
            std::error_code error;
            fs::path path = fs::canonical(item.path(), error);
            if (!error) {
                if (fs::is_directory(item)) {
                    journal->append(JournalMethod::ADDED, JournalTarget::DIRECTORY,
                                    pathTransformer->transformToJournalFormat(path),
                                    FileData(item.path()));
                } else {
                    journal->append(JournalMethod::ADDED, JournalTarget::FILE,
                                    pathTransformer->transformToJournalFormat(path),
                                    FileData(item.path()));
                }
            } else {
                LOGGER("error occured: " + error.message());
            }
        }
    });

    journal->setFunc(JournalMethod::MODIFIED, JournalTarget::DIRECTORY, [&](auto &i) {
        fs::path dirPath = pathTransformer->transformFromJournalFormat(i.getPath());

        auto &fileMap = getFileMap();
        for (const auto &item : fs::directory_iterator(dirPath)) {
            fs::path path = fs::canonical(item.path());
            auto &attr = fileMap[path];
            if (fs::exists(path)) {
                auto currentFileTime = fs::last_write_time(path);
                if (!attr) {
                    //I don't like the fact that this logic is in two places, here and update().
                    if (!fileMap.isDeleted(fs::canonical(item.path())) ||
                        currentFileTime > fileMap.getDeletionTime(path)) {
                        journal->append(JournalMethod::ADDED,
                                        fs::is_directory(path) ? JournalTarget::DIRECTORY : JournalTarget::FILE,
                                        pathTransformer->transformToJournalFormat(path),
                                        FileData(item.path()));
                    }
                }
            } else {
                if (attr) {
                    journal->append(JournalMethod::DELETED,
                                    attr->isDirectory() ? JournalTarget::DIRECTORY : JournalTarget::FILE,
                                    pathTransformer->transformToJournalFormat(path),
                                    attr->toFileData(path));
                }

            }
        }
    });

    journal->setFunc(JournalMethod::DELETED, JournalTarget::DIRECTORY, [&](auto &i) {
        //@todo delete everything recursively
        auto subMap = getFileMap().subMap(pathTransformer->transformFromJournalFormat(i.getPath()));
        for (const auto &[subPath, value] : subMap) {
            //this will not set the isDirectory flag, but I don't think it's important.
            // @todo maybe I should split METHOD into two: METHOD and TARGET (DELETED, DIRECTORY)
            if (value->isDirectory()) {
                journal->append(JournalMethod::DELETED, JournalTarget::DIRECTORY,
                                pathTransformer->transformToJournalFormat(subPath),
                                getFileMap()[subPath]->toFileData(subPath));
            } else {
                journal->append(JournalMethod::DELETED, JournalTarget::FILE,
                                pathTransformer->transformToJournalFormat(subPath),
                                getFileMap()[subPath]->toFileData(subPath));
            }
        }
    });


    journal->setFunc(JournalMethod::IGNORED, JournalTarget::FILE, [&](auto &i) {
        fs::path filePath = pathTransformer->transformFromJournalFormat(i.getPath());
        auto &fileMap = getFileMap();

        if (fileMap[filePath]) {
            journal->append(JournalMethod::FORGOTTEN, JournalTarget::FILE,
                            filePath,
                            fileMap[filePath]->toFileData(filePath));
        }

    });

    journal->setFunc(JournalMethod::IGNORED, JournalTarget::DIRECTORY, [&](auto &i) {
        fs::path dirPath = pathTransformer->transformFromJournalFormat(i.getPath());
        LOGGER("ignoring dir ... " + dirPath.string())
        auto subMap = getFileMap().subMap(dirPath);
        for (const auto &[subPath, value] : subMap) {
            //this will not set the isDirectory flag, but I don't think it's important.
            LOGGER("subpath " + subPath.string())
            journal->append(JournalMethod::FORGOTTEN,
                            value->isDirectory() ? JournalTarget::DIRECTORY : JournalTarget::FILE,
                            pathTransformer->transformToJournalFormat(subPath),
                            getFileMap()[subPath]->toFileData(subPath));
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
        auto target = !attr->isDirectory() ? JournalTarget::FILE : JournalTarget::DIRECTORY;
        //@todo check if file was actually changed.
        journal->append(JournalMethod::MODIFIED, target,
                        pathTransformer->transformToJournalFormat(path),
                        FileData(path));


    } else {
        auto target = !fs::is_directory(path) ? JournalTarget::FILE : JournalTarget::DIRECTORY;


        journal->append(JournalMethod::ADDED, target,
                        pathTransformer->transformToJournalFormat(path),
                        FileData(path));


    }
}

void Repository::downloadStorage() {

    auto &fileMap = getFileMap();
    bool hasResources = true;
    for (const auto &[path, value] : fileMap) {
        LOGGER("file " + path.string() + " => " + (value ? value->getResourceId() : std::string(" [X] ")));
        //@todo change this bool to actual transfer management

        //check for resources.
        if (value && !value->isDirectory()) {
            if (!storage->hasResource(value->getResourceId())) {
                hasResources &= storage->acquireResource(value->getResourceId());
            }
        }


    }
    //@todo wait for finish?

}

void Repository::update(fs::path path, const RepositoryActionStrategyPack &strategyPack) {
    //@todo change the name of this function to something else. it's about merging the state or journal and filesystem and updating one from the other or both
    path = fs::weakly_canonical(path);
    auto &fileMap = getFileMap();
    //only update if the file is in the repository
    auto &attributes = fileMap[path];
    if (fs::exists(path)) {
        //file exists in filesystem
        auto currentFileTime = fs::last_write_time(path);
        auto currentFileSize = !fs::is_directory(path) ? fs::file_size(path) : 0;
        if (attributes) {
            //file exists in the journal
//            LOGGER("current time " + std::to_string(currentFileTime) + " lwt " +
//                   std::to_string(attributes->getModificationTime()))
            if (currentFileTime < attributes->getModificationTime()) {
                //file in repository is newer than the file in filesystem, restore
                LOGGER("restoring..." + path.string())
                auto state = strategyPack.updatedInRepo->apply(path, attributes);
                //@todo fix deployed by using return value from strategy->apply
                deployMap.markDeployed(path, state);
            } else {
                if (currentFileTime == attributes->getModificationTime() && currentFileSize == attributes->getSize()) {
                    //@todo verify other things maybe
                    //file appear identical, nothing to do.
                    LOGGER("leaving alone " + path.string())
                    auto state = strategyPack.same->apply(path, attributes);
                    deployMap.markDeployed(path, state);
                } else {
                    LOGGER("updated file, persisting... " + path.string())
                    //file in the filesystem is newer then the one in repository
                    auto state = strategyPack.updatedInFilesystem->apply(path, attributes);
                    deployMap.markDeployed(path, state);
                }
            }
        } else {
            //not in the map

//            LOGGER("cur fil tim " + std::to_string(currentFileTime) + " deltim " +
//                   std::to_string(fileMap.getDeletionTime(path)))
            if (currentFileTime > fileMap.getDeletionTime(path) || !fileMap.isDeleted(path)) {
                //this is new file that has the same path as deleted one. persist!
                LOGGER("new file, persisting " + path.string())
                auto state = strategyPack.newInFilesystem->apply(path);
                deployMap.markDeployed(path, state);

            } else {
                //@todo delete file
                if (fileMap.isDeleted(path)) {
                    LOGGER("trashing " + path.string())
                    //file should be deleted, but let's check deletion time...
                    auto state = strategyPack.deletedInRepo->apply(path);
                    deployMap.markDeployed(path, state);
                }
            }


        }
    } else {
        //file was removed or perhaps wasn't deployed yet...
        //@todo not so sure about that...
        if (attributes) {
            if (deployMap.isDeployed(path)) {
                //file is in file map and was deployed but is not on filesystem, removing (user must have deleted it)
                LOGGER("deleting " + path.string());
                auto state = strategyPack.deletedInFilesystem->apply(path, attributes);
                deployMap.markDeployed(path, state);
            } else {
                //file wasn't deployed. must be a new file from another node. restore.
                //@todo this is duplicated code. move to strategies or sth. (three times as of writing this. seriously, do something about it)
                auto state = strategyPack.newInRepo->apply(path, attributes);
                deployMap.markDeployed(path, state);
            }
        } else {
            //file is neither on file system nor in the map. someone trying to update unknown file?
            LOGGER("file unknown " + path.string());
            //@todo add strategy for this ?
        }
    }


}

void Repository::syncLocalChanges() {
//@todo change the name of this function to updateAll or sth
    for (const auto &i : getFileMap()) {
        update(i.first, localSyncPack);
    }

}

void Repository::restoreAttributes(const fs::path &path) {

    auto &fileMap = getFileMap();

    if (fs::exists(path)) {
        auto &attributes = fileMap[path];
        fs::permissions(path, attributes->getPermissions());
        fs::last_write_time(path, attributes->getModificationTime());
    }

}

void Repository::forget(fs::path path) {

    auto &fileMap = getFileMap();
    if (path.is_relative()) {
        //@todo not so sure about current path, i have to make sure this is always set to the right value
        path = fs::canonical(fs::current_path() / path);
    }
    auto &attr = fileMap[path];
    if (attr) {
        journal->append(JournalMethod::FORGOTTEN, attr->isDirectory() ? JournalTarget::DIRECTORY : JournalTarget::FILE,
                        pathTransformer->transformToJournalFormat(path),
                        attr->toFileData(path));
    } else {
        //nothing to forget!
    }
}

void Repository::remove(fs::path path) {

    auto &fileMap = getFileMap();
    if (path.is_relative()) {
        //@todo not so sure about current path, i have to make sure this is always set to the right value
        path = fs::canonical(fs::current_path() / path);
    }
    auto &attr = fileMap[path];
    if (attr) {
        if (!fs::is_directory(path)) {
            journal->append(JournalMethod::DELETED, JournalTarget::FILE,
                            pathTransformer->transformToJournalFormat(path),
                            attr->toFileData(path));

        } else {
            journal->append(JournalMethod::DELETED, JournalTarget::DIRECTORY,
                            pathTransformer->transformToJournalFormat(path),
                            attr->toFileData(path));
            //@todo delete everything recursively ... or maybe do it in replayCurrentState?

        }
    } else {
        //nothing to forget!
    }
}

void Repository::ignore(fs::path path) {
//    auto &fileMap = getFileMap();
    if (path.is_relative()) {
        //@todo not so sure about current path, i have to make sure this is always set to the right value
        path = fs::canonical(fs::current_path() / path);
    }
    if (!fs::is_directory(path)) {
        journal->append(JournalMethod::IGNORED, JournalTarget::FILE, pathTransformer->transformToJournalFormat(path),
                        FileData(path));

    } else {
        journal->append(JournalMethod::IGNORED, JournalTarget::DIRECTORY,
                        pathTransformer->transformToJournalFormat(path),
                        FileData(path));
    }
}

void Repository::trash(const fs::path &path) {

    //@todo implement trashing
    fs::remove(path);

}

void Repository::deploy() {
    for (const auto &i : getFileMap()) {
        //@todo add force level, by using different pack
        update(i.first, deployPack);
    }
}

void Repository::RepoFileMap::prepareMap() {
//    LOGGER("prepare map jch:" + journal->getChecksum() + " mck " + mapChecksum)

    if (mapChecksum != journal->getChecksum()) {
        LOGGER("checksum different, recreate file map")
        attributesMap.clear();
        journal->clearFunc();
        journal->setFunc(JournalMethod::ADDED, JournalTarget::FILE, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = Attributes(i);
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::MODIFIED, JournalTarget::FILE, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = Attributes(i);
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        //@todo moved file should have two parameters - from to. or, just remove MOVED and use DELETED/ADDED
        journal->setFunc(JournalMethod::MOVED, JournalTarget::FILE, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = Attributes(i);
        });

        journal->setFunc(JournalMethod::DELETED, JournalTarget::FILE, [&](auto &i) {
            auto path = pathTransformer->transformFromJournalFormat(i.getPath());
            attributesMap[path] = std::nullopt;
            deleteMap[path].setDeleted(true);
            deleteMap[path].setDeletionTime(i.getModificationTime());
//            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::FORGOTTEN, JournalTarget::FILE, [&](auto &i) {
            auto path = pathTransformer->transformFromJournalFormat(i.getPath());
            attributesMap[path] = std::nullopt;
            deleteMap[path].setDeleted(false);
            deleteMap[path].setDeletionTime(fs::file_time_type::min());
//            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::ADDED, JournalTarget::DIRECTORY, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = Attributes(i);
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::MODIFIED, JournalTarget::DIRECTORY, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = Attributes(i);
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        //@todo moved file should have two parameters - from to. or, just remove MOVED and use DELETED/ADDED
        journal->setFunc(JournalMethod::MOVED, JournalTarget::DIRECTORY, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = Attributes(i);
        });

        journal->setFunc(JournalMethod::DELETED, JournalTarget::DIRECTORY, [&, this](auto &i) {
            auto path = pathTransformer->transformFromJournalFormat(i.getPath());
            attributesMap[path] = std::nullopt;
            deleteMap[path].setDeleted(true);
            deleteMap[path].setDeletionTime(i.getModificationTime());
//            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::FORGOTTEN, JournalTarget::DIRECTORY, [&](auto &i) {
            auto path = pathTransformer->transformFromJournalFormat(i.getPath());
            attributesMap[path] = std::nullopt;
            deleteMap[path].setDeleted(false);
            deleteMap[path].setDeletionTime(fs::file_time_type::min());
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

auto Repository::RepoFileMap::getSize(const fs::path &path) {
    return attributesMap[path]->getSize();
}

decltype(Repository::RepoFileMap::attributesMap) Repository::RepoFileMap::subMap(const fs::path &root) {
    decltype(Repository::RepoFileMap::attributesMap) result;
    for (const auto &[path, value] : attributesMap) {
        if (path.string().find(root.string()) != std::string::npos) {
            result[path] = value;
        }
    }
    return result;
}

fs::file_time_type Repository::RepoFileMap::getDeletionTime(const fs::path &path) {
    return deleteMap[path].getDeletionTime();
}

auto Repository::RepoFileMap::isDeleted(const fs::path &path) -> decltype(deleteMap[path].isDeleted()) {
    return deleteMap[path].isDeleted();
}

fs::perms Repository::RepoFileMap::Attributes::getPermissions() const {
    return permissions;
}

uintmax_t Repository::RepoFileMap::Attributes::getSize() const {
    return size;
}

fs::file_time_type Repository::RepoFileMap::Attributes::getModificationTime() const {
    return modificationTime;
}

const ChecksumId &Repository::RepoFileMap::Attributes::getChecksum() const {
    return checksum;
}

const IStorage::ResourceId &Repository::RepoFileMap::Attributes::getResourceId() const {
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


bool Repository::RepoFileMap::DeleteInfo::isDeleted() const {
    return deleted;
}

fs::file_time_type Repository::RepoFileMap::DeleteInfo::getDeletionTime() const {
    return deletionTime;
}

void Repository::RepoFileMap::DeleteInfo::setDeletionTime(fs::file_time_type deletionTime) {
    DeleteInfo::deletionTime = deletionTime;
}

void Repository::RepoFileMap::DeleteInfo::setDeleted(bool deleted) {
    DeleteInfo::deleted = deleted;
}

auto Repository::RepoDeployMap::begin() -> decltype(deployMap.begin()) {
    return deployMap.begin();
}

auto Repository::RepoDeployMap::end() -> decltype(deployMap.end()) {
    return deployMap.end();
}

auto Repository::RepoDeployMap::operator[](const fs::path &path) -> decltype(deployMap[fs::current_path()]) {
    return deployMap[path];
}

bool Repository::RepoDeployMap::DeployAttributes::isDeployed() const {
    return deployed;
}

void Repository::RepoDeployMap::DeployAttributes::setDeployed(bool deployed) {
    DeployAttributes::deployed = deployed;
}

//@todo maybe move actual implementation code from methods like repository.persist, delete, trash... etc. to those strategies? just a thought

class StrategyPersist : public Repository::RepositoryActionStrategy {
    Repository::DeployState
    apply(const fs::path &path, const std::optional<Repository::RepoFileMap::Attributes> &attributes) override {
        repository.persist(path);
        return Repository::DeployState::UNCHANGED;
    }

public:
    StrategyPersist(Repository &repository) : RepositoryActionStrategy(repository) {}


};

class StrategyRestore : public Repository::RepositoryActionStrategy {
public:
    Repository::DeployState
    apply(const fs::path &path, const std::optional<Repository::RepoFileMap::Attributes> &attributes) override {
        if (!attributes->isDirectory()) {
            repository.getStorage()->restore(attributes->getResourceId(), path);
        }
        repository.restoreAttributes(path);
        return Repository::DeployState::DEPLOYED;
    }

    StrategyRestore(Repository &repository) : RepositoryActionStrategy(repository) {}

};

class StrategyTrash : public Repository::RepositoryActionStrategy {
public:
    Repository::DeployState
    apply(const fs::path &path, const std::optional<Repository::RepoFileMap::Attributes> &attributes) override {
        repository.trash(path);
        return Repository::DeployState::NOT_DEPLOYED;
    }

    StrategyTrash(Repository &repository) : RepositoryActionStrategy(repository) {}

};

class StrategyRemove : public Repository::RepositoryActionStrategy {
public:
    Repository::DeployState
    apply(const fs::path &path, const std::optional<Repository::RepoFileMap::Attributes> &attributes) override {
        repository.remove(path);
        return Repository::DeployState::NOT_DEPLOYED;
    }

    StrategyRemove(Repository &repository) : RepositoryActionStrategy(repository) {}

};

class StrategyDelete : public Repository::RepositoryActionStrategy {
public:
    Repository::DeployState
    apply(const fs::path &path, const std::optional<Repository::RepoFileMap::Attributes> &attributes) override {
        //@todo implement
        return Repository::DeployState::NOT_DEPLOYED;
    }

    StrategyDelete(Repository &repository) : RepositoryActionStrategy(repository) {}

};

class StrategyNull : public Repository::RepositoryActionStrategy {
public:
    Repository::DeployState
    apply(const fs::path &path, const std::optional<Repository::RepoFileMap::Attributes> &attributes) override {
        return Repository::DeployState::UNCHANGED; //@todo bool is stupid thing to return. even enum would be better. even optional<bool> would be better!
    }

    StrategyNull(Repository &repository) : RepositoryActionStrategy(repository) {}

};

std::shared_ptr<Repository::RepositoryActionStrategy>
Repository::RepositoryActionStrategyFactory::create(Repository::RepositoryAction action) {
    switch (action) {
        case RepositoryAction::PERSIST:
        case RepositoryAction::UPDATE:
            //update and persist are the same
            return std::make_shared<StrategyPersist>(repository);
        case RepositoryAction::DELETE:
            return std::make_shared<StrategyDelete>(repository);
        case RepositoryAction::TRASH:
            return std::make_shared<StrategyTrash>(repository);
        case RepositoryAction::REMOVE:
            return std::make_shared<StrategyRemove>(repository);
        case RepositoryAction::RESTORE:
            return std::make_shared<StrategyRestore>(repository);
        default: //<---- this is here just so that compiler won't complain about return reaching end of function.
        case RepositoryAction::NOP:
            return std::make_shared<StrategyNull>(repository);
    }
}

Repository::RepositoryActionStrategyFactory::RepositoryActionStrategyFactory(Repository &repository) : repository(
        repository) {}

Repository::RepositoryActionStrategyPack
Repository::RepositoryActionStrategyFactory::createPack(Repository::RepoActionPack actionPack) {
    Repository::RepositoryActionStrategyPack strategyPack;
    strategyPack.updatedInRepo = create(actionPack.updatedInRepo);
    strategyPack.updatedInFilesystem = create(actionPack.updatedInFilesystem);
    strategyPack.same = create(actionPack.same);
    strategyPack.newInRepo = create(actionPack.newInRepo);
    strategyPack.newInFilesystem = create(actionPack.newInFilesystem);
    strategyPack.deletedInRepo = create(actionPack.deletedInRepo);
    strategyPack.deletedInFilesystem = create(actionPack.deletedInFilesystem);
    return strategyPack;
}
