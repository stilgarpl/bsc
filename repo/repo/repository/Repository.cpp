#include <utility>

//
// Created by stilgar on 17.10.17.
//

#include <repo/repository/transformer/rules/TmpRule.h>
#include <repo/repository/transformer/rules/HomeDirRule.h>
#include "Repository.h"
#include <repo/repository/storage/InternalStorage.h>

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

Repository::Repository(RepoIdType repositoryId, IStoragePtr storagePtr) : strategyFactory(*this),
                                                                          repositoryId(std::move(repositoryId)),
                                                                          storage(std::move(storagePtr)),
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

    journal->setFunc(JournalMethod::MODIFIED, JournalTarget::FILE, [&](auto &i) {
        storage->store(calculateSha1OfFile(pathTransformer->transformFromJournalFormat(i.getPath())),
                       fs::file_size(pathTransformer->transformFromJournalFormat(i.getPath())),
                       pathTransformer->transformFromJournalFormat(i.getPath()));
        LOGGER("commit: modified file " + pathTransformer->transformFromJournalFormat(i.getPath()).string())
    });

    journal->replayCurrentState();
    journal->commitState(CommitTimeType::clock::now());
}

void Repository::persist(fs::path path) {
    LOGGER("persist: " + path.string())
    auto &fileMap = getFileMap();
    if (path.is_relative()) {
        path = fs::canonical(fs::current_path() / path);
        LOGGER("after canonical: " + path.string())
    }

    auto &attr = fileMap[path];
    if (attr) {
        //file exists in map! update mode
        auto target = !attr->isDirectory() ? JournalTarget::FILE : JournalTarget::DIRECTORY;
        //@todo check if file was actually changed.
        journal->append(JournalMethod::MODIFIED, target,
                        pathTransformer->transformToJournalFormat(path).string(),
                        FileData(path));


    } else {
        auto target = !fs::is_directory(path) ? JournalTarget::FILE : JournalTarget::DIRECTORY;


        journal->append(JournalMethod::ADDED, target,
                        pathTransformer->transformToJournalFormat(path).string(),
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
            LOGGER("checking if file should be downloaded")
            if (!storage->hasResource(value->getResourceId())) {
                LOGGER("storage doesn't have resource " + value->getResourceId() + " , acquiring")
                hasResources &= storage->acquireResource(value->getResourceId());
            }
        }


    }
    auto queue = storage->getTransferQueue();
    if (queue != nullptr) {
        queue->waitToFinishAllTransfers();
    }

}

void Repository::update(fs::path path, const RepositoryActionStrategyPack &strategyPack,
                        std::set<UpdateOptions> updateOptions) {
    //@todo change the name of this function to something else. it's about merging the state or journal and filesystem and updating one from the other or both
    LOGGER("update: " + path.string())
    path = fs::weakly_canonical(path);
    auto &fileMap = getFileMap();
    //only update if the file is in the repository
//    if (fileMap.contains(path)) {

    if (fs::exists(path)) {
        if (fs::is_directory(path) || fs::is_regular_file(path)) {
            //file exists in filesystem
            auto currentFileTime = fs::last_write_time(path);
            auto currentFileSize = !fs::is_directory(path) ? fs::file_size(path) : 0;
            if (fileMap.contains(path) && fileMap[path].has_value()) {
                auto &attributes = fileMap[path];
                //file exists in the journal
                if (currentFileTime < attributes->getModificationTime()) {
                    //file in repository is newer than the file in filesystem, restore
                    LOGGER("restoring..." + path.string())
                    auto state = strategyPack.updatedInRepo->apply(path, attributes);
                    deployMap.markDeployed(path, state);
                } else {
                    if (currentFileTime == attributes->getModificationTime() &&
                        currentFileSize == attributes->getSize()) {
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

                        //@todo this is mostly the same code as below, combine the two loops into a function or sth.
                        if (fs::is_directory(path) &&
                            (updateOptions.contains(UpdateOptions::FOLLOW_UPDATED_DIRECTORIES) ||
                             updateOptions.contains(UpdateOptions::FOLLOW_DIRECTORIES))) {
                            LOGGER(path.string() + " is a directory, iterating over...")
                            for (const auto &item : fs::directory_iterator(path)) {
                                //make sure item is not in the fileMap - if it is, update will be called for it anyway... but only if called from updateAll. @todo maybe add flag to force recursive behavior? or not -- if we are assuming that it will always be called from a loop.

                                LOGGER(" item: " + item.path().string())
                                if (!fileMap.contains(item)) {
                                    LOGGER("following directory " + path.string() + " to " + item.path().string())
                                    update(item, strategyPack, updateOptions);
                                }
                            }
                        }

                    }
                }
            } else {
                //not in the map
                if (!fileMap.isDeleted(path) || currentFileTime > fileMap.getDeletionTime(path)) {
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
            //@todo this iterates over all directories and files in path, which means that it will probably call some methods twice or even more times on some files. make sure it doesn't
            if (fs::is_directory(path) && updateOptions.contains(UpdateOptions::FOLLOW_DIRECTORIES)) {
                LOGGER(path.string() + " is a directory, iterating over...")
                for (const auto &item : fs::directory_iterator(path)) {
                    //make sure item is not in the fileMap - if it is, update will be called for it anyway... but only if called from updateAll. @todo maybe add flag to force recursive behavior? or not -- if we are assuming that it will always be called from a loop.

                    LOGGER(" item: " + item.path().string())
                    if (!fileMap.contains(item)) {
                        LOGGER("following directory " + path.string() + " to " + item.path().string())
                        update(item, strategyPack, updateOptions);
                    }
                }
            }
        } else {
            LOGGER("file type not supported for : " + path.string())
        }
    } else {
        //file was removed or perhaps wasn't deployed yet...
        //@todo not so sure about that...
        if (fileMap.contains(path) && fileMap[path].has_value()) {
            auto &attributes = fileMap[path];
            if (deployMap.isDeployed(path)) {
                //file is in file map and was deployed but is not on filesystem, removing (user must have deleted it)
                LOGGER("deleting " + path.string());
                auto state = strategyPack.deletedInFilesystem->apply(path, attributes);
                deployMap.markDeployed(path, state);
            } else {
                //file wasn't deployed. must be a new file from another node. restore.
                //@todo this is duplicated code. move to strategies or sth. (three times as of writing this. seriously, do something about it)
                LOGGER("not deployed " + path.string())
                auto state = strategyPack.newInRepo->apply(path, attributes);
                deployMap.markDeployed(path, state);
            }
        } else {
            if (fileMap.isDeleted(path)) {
                LOGGER("file deleted, but not in filesystem so nothing to do: " + path.string())
                deployMap.markDeployed(path, DeployState::NOT_DEPLOYED);
            } else {
                //file is neither on file system nor in the map. someone trying to update unknown file?
                LOGGER("file unknown " + path.string());
                //@todo add strategy for this ?
            }
        }
    }

//    }
}

void Repository::syncLocalChanges() {
//@todo change the name of this function to updateAll or sth
    for (const auto &i : getFileMap()) {
        LOGGER("update()" + i.first.string())
        update(i.first, localSyncPack, {UpdateOptions::FOLLOW_UPDATED_DIRECTORIES});
    }


}

void Repository::restoreAttributes(const fs::path &path) {

    auto &fileMap = getFileMap();

    if (fs::exists(path)) {
        if (fileMap.contains(path)) {
            auto &attributes = fileMap[path];
            if (attributes) {
                fs::permissions(path, attributes->getPermissions());
                fs::last_write_time(path, attributes->getModificationTime());
            }
        }
    }

}

void Repository::forget(fs::path path) {

    auto &fileMap = getFileMap();
    if (path.is_relative()) {
        //@todo not so sure about current path, i have to make sure this is always set to the right value
        path = fs::canonical(fs::current_path() / path);
    }
    if (fileMap.contains(path)) {
        auto &attr = fileMap[path];
        if (attr) {
            journal->append(JournalMethod::FORGOTTEN,
                            attr->isDirectory() ? JournalTarget::DIRECTORY : JournalTarget::FILE,
                            pathTransformer->transformToJournalFormat(path).string(),
                            attr->toFileData(path));
        } else {
            //nothing to forget!
        }
    }
}

void Repository::remove(fs::path path) {

    auto &fileMap = getFileMap();
    if (path.is_relative()) {
        //@todo not so sure about current path, i have to make sure this is always set to the right value
        path = fs::canonical(fs::current_path() / path);
    }
    if (fileMap.contains(path)) {
        auto &attr = fileMap[path];
        if (attr) {
            if (!fs::is_directory(path)) {
                journal->append(JournalMethod::DELETED, JournalTarget::FILE,
                                pathTransformer->transformToJournalFormat(path).string(),
                                attr->toFileData(path));

            } else {
                journal->append(JournalMethod::DELETED, JournalTarget::DIRECTORY,
                                pathTransformer->transformToJournalFormat(path).string(),
                                attr->toFileData(path));
                //@todo delete everything recursively ... or maybe do it in replayCurrentState?

            }
        } else {
            //nothing to forget!
        }
    }
}

void Repository::ignore(fs::path path) {
//    auto &fileMap = getFileMap();
    if (path.is_relative()) {
        //@todo not so sure about current path, i have to make sure this is always set to the right value
        path = fs::canonical(fs::current_path() / path);
    }
    if (!fs::is_directory(path)) {
        journal->append(JournalMethod::IGNORED, JournalTarget::FILE,
                        pathTransformer->transformToJournalFormat(path).string(),
                        FileData(path));

    } else {
        journal->append(JournalMethod::IGNORED, JournalTarget::DIRECTORY,
                        pathTransformer->transformToJournalFormat(path).string(),
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
        update(i.first, deployPack, {});
    }
}

Repository::Repository(const IRepository::RepoIdType &repositoryId, const std::shared_ptr<IStorage> &storage,
                       const JournalPtr &journal, const Repository::RepoDeployMap &deployMap) : Repository(repositoryId,
                                                                                                           storage) {
    this->journal = journal;
    this->deployMap = deployMap;
}

const IRepository::RepositoryActionStrategyPack &Repository::getDeployPack() const {
    return deployPack;
}

const IRepository::RepositoryActionStrategyPack &Repository::getLocalSyncPack() const {
    return localSyncPack;
}

const IRepository::RepositoryActionStrategyPack &Repository::getFullPack() const {
    return fullPack;
}

//Repository::Repository() : Repository("", nullptr) {}

void Repository::RepoFileMap::prepareMap() {
//    LOGGER("prepare map jch:" + journal->getChecksum() + " mck " + mapChecksum)

    if (mapChecksum != journal->getChecksum()) {
        LOGGER("checksum different, recreate file map")
        attributesMap.clear();
        journal->clearFunc();
        journal->setFunc(JournalMethod::ADDED, JournalTarget::FILE, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = RepositoryAttributes(i);
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::MODIFIED, JournalTarget::FILE, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = RepositoryAttributes(i);
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        //@todo moved file should have two parameters - from to. or, just remove MOVED and use DELETED/ADDED
        journal->setFunc(JournalMethod::MOVED, JournalTarget::FILE, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = RepositoryAttributes(i);
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
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = RepositoryAttributes(i);
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        journal->setFunc(JournalMethod::MODIFIED, JournalTarget::DIRECTORY, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = RepositoryAttributes(i);
            LOGGER(IStorage::getResourceId(i.getChecksum(), i.getSize()) + " ::: " + i.getPath());
        });

        //@todo moved file should have two parameters - from to. or, just remove MOVED and use DELETED/ADDED
        journal->setFunc(JournalMethod::MOVED, JournalTarget::DIRECTORY, [&](auto &i) {
            attributesMap[pathTransformer->transformFromJournalFormat(i.getPath())] = RepositoryAttributes(i);
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

auto Repository::RepoDeployMap::operator[](const fs::path& path) -> decltype(deployMap[fs::current_path().string()]) {
    return deployMap[path.string()];
}

void Repository::RepoDeployMap::markDeployed(const fs::path &path, Repository::DeployState deployState) {
    if (deployState == DeployState::DEPLOYED) {
        LOGGER("marking " + path.string() + " as deployed")
        deployMap[path.string()] = true;
    } else if (deployState == DeployState::NOT_DEPLOYED) {
        LOGGER("marking " + path.string() + " as not deployed")
        deployMap[path.string()] = false;
    } //else unchanged
}

bool Repository::RepoDeployMap::DeployAttributes::isDeployed() const {
    return deployed;
}

void Repository::RepoDeployMap::DeployAttributes::setDeployed(bool deployed) {
    LOGGER("set deployed")
    DeployAttributes::deployed = deployed;
}

Repository::RepoDeployMap::DeployAttributes::DeployAttributes(bool deployed) : deployed(deployed) {}

Repository::RepoDeployMap::DeployAttributes::DeployAttributes() = default;

//@todo maybe move actual implementation code from methods like repository.persist, delete, trash... etc. to those strategies? just a thought

class StrategyPersist : public Repository::RepositoryActionStrategy {
    Repository::DeployState
    apply(const fs::path &path, const std::optional<RepositoryAttributes> &attributes) override {
        repository.persist(path);
        return Repository::DeployState::DEPLOYED; //it was UNCHANGED, but file is clearly in the filesystem, so it's deployed!
        //UNCHANGED made some sense, because persisting the file doesn't actually deploy it, but new files weren't marked as deployed, so update thought they were deleted. and new files go through persist, so it should return DEPLOYED.
    }

public:
    StrategyPersist(Repository &repository) : RepositoryActionStrategy(repository) {}


};

class StrategyRestore : public Repository::RepositoryActionStrategy {
public:
    Repository::DeployState
    apply(const fs::path &path, const std::optional<RepositoryAttributes> &attributes) override {
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
    apply(const fs::path &path, const std::optional<RepositoryAttributes> &attributes) override {
        repository.trash(path);
        return Repository::DeployState::NOT_DEPLOYED;
    }

    StrategyTrash(Repository &repository) : RepositoryActionStrategy(repository) {}

};

class StrategyRemove : public Repository::RepositoryActionStrategy {
public:
    Repository::DeployState
    apply(const fs::path &path, const std::optional<RepositoryAttributes> &attributes) override {
        repository.remove(path);
        return Repository::DeployState::NOT_DEPLOYED;
    }

    StrategyRemove(Repository &repository) : RepositoryActionStrategy(repository) {}

};

class StrategyDelete : public Repository::RepositoryActionStrategy {
public:
    Repository::DeployState
    apply(const fs::path &path, const std::optional<RepositoryAttributes> &attributes) override {
        //@todo implement
        return Repository::DeployState::NOT_DEPLOYED;
    }

    StrategyDelete(Repository &repository) : RepositoryActionStrategy(repository) {}

};

class StrategyNull : public Repository::RepositoryActionStrategy {
public:
    Repository::DeployState
    apply(const fs::path &path, const std::optional<RepositoryAttributes> &attributes) override {
        return Repository::DeployState::UNCHANGED;
    }

    StrategyNull(Repository &repository) : RepositoryActionStrategy(repository) {}

};

std::shared_ptr<Repository::RepositoryActionStrategy>
Repository::RepositoryActionStrategyFactory::create(Repository::RepositoryAction action) const {
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
Repository::RepositoryActionStrategyFactory::createPack(Repository::RepoActionPack actionPack) const {
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
