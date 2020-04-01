#include <utility>

//
// Created by stilgar on 17.10.17.
//

#include "Repository.h"
#include <repo/repository/transformer/rules/HomeDirRule.h>
#include <repo/repository/transformer/rules/TmpRule.h>


#include <repo/repository/storage/InternalStorage.h>
namespace bsc {
    using namespace std::chrono_literals;

    const IRepository::RepoIdType& Repository::getRepositoryId() const {
        return repositoryId;
    }


    JournalPtr& Repository::getJournal() {
        return journal;
    }

    void Repository::setJournal(const JournalPtr& journal) {
        Repository::journal = journal;
    }

    Repository::Repository(RepoIdType repositoryId, IStoragePtr storagePtr) : manipulator(*this),
                                                                              repositoryId(std::move(repositoryId)),
                                                                              storage(std::move(storagePtr)),
                                                                              deployPack(strategyFactory.createPack(
                                                                                      {
                                                                                              .updatedInRepo = RepositoryAction::restore,
                                                                                              .updatedInFilesystem = RepositoryAction::nop,
                                                                                              .same = RepositoryAction::nop,
                                                                                              .newInRepo = RepositoryAction::restore,
                                                                                              .newInFilesystem = RepositoryAction::nop,
                                                                                              .deletedInRepo = RepositoryAction::trash,
                                                                                              .deletedInFilesystem = RepositoryAction::restore,
                                                                                      })),
                                                                              localSyncPack(strategyFactory.createPack(
                                                                                      {
                                                                                              .updatedInRepo = RepositoryAction::nop,
                                                                                              .updatedInFilesystem = RepositoryAction::update,
                                                                                              .same = RepositoryAction::nop,
                                                                                              .newInRepo = RepositoryAction::nop,
                                                                                              .newInFilesystem = RepositoryAction::persist,
                                                                                              .deletedInRepo = RepositoryAction::nop,
                                                                                              .deletedInFilesystem = RepositoryAction::remove,
                                                                                      })),
                                                                              fullPack(strategyFactory.createPack(
                                                                                      {
                                                                                              .updatedInRepo = RepositoryAction::restore,
                                                                                              .updatedInFilesystem = RepositoryAction::update,
                                                                                              .same = RepositoryAction::nop,
                                                                                              .newInRepo = RepositoryAction::restore,
                                                                                              .newInFilesystem = RepositoryAction::persist,
                                                                                              .deletedInRepo = RepositoryAction::trash,
                                                                                              .deletedInFilesystem = RepositoryAction::remove,
                                                                                      })),
                                                                              fileMapRenderer(pathTransformer) {
        pathTransformer->addRule(std::make_shared<TmpRule>());
        pathTransformer->addRule(std::make_shared<HomeDirRule>());
    }

    const std::shared_ptr<IStorage>& Repository::getStorage() const {
        return storage;
    }

    void Repository::restoreAll() {

        //@todo this method kind of does the same as deploy() merge the two
        auto& fileMap = fileMapRenderer.renderMap(journal);
        for (auto&& [path, attributes] : fileMap) {
            if (attributes) {
                LOGGER("restoring path " + path.string())
                manipulator.restoreFileFromStorage(path,attributes);
                deployMap.markDeployed(path, DeployState::deployed);
                manipulator.restoreAttributes(path, attributes);
            } else {
                LOGGER("resAll: no value")
            }
        }
    }

    void Repository::commit() {
            manipulator.commit(*storage);
    }


    void Repository::downloadStorage() {

        auto& fileMap = fileMapRenderer.renderMap(journal);
        bool hasResources = true;
        for (const auto& [path, value] : fileMap) {
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

    void Repository::update(fs::path path, const RepositoryActionStrategyPack& strategyPack,
                            std::set<UpdateOptions> updateOptions) {
        //@todo change the name of this function to something else. it's about merging the state or journal and filesystem and updating one from the other or both
        LOGGER("update: " + path.string())
        path = fs::weakly_canonical(path);
        auto& fileMap = fileMapRenderer.renderMap(journal);
        //only update if the file is in the repository
        //    if (fileMap.contains(path)) {

        if (fs::exists(path)) {
            if (fs::is_directory(path) || fs::is_regular_file(path)) {
                //file exists in filesystem
                auto currentFileTime = fs::last_write_time(path);
                auto currentFileSize = !fs::is_directory(path) ? fs::file_size(path) : 0;
                if (fileMap[path].has_value()) {
                    auto& attributes = fileMap[path];
                    //file exists in the journal
                    if (currentFileTime < attributes->getModificationTime()) {
                        //file in repository is newer than the file in filesystem, restore
                        LOGGER("restoring..." + path.string())
                        auto state = strategyPack.updatedInRepo->apply(path, attributes, manipulator);
                        deployMap.markDeployed(path, state);
                    } else {
                        if (currentFileTime == attributes->getModificationTime() &&
                            currentFileSize == attributes->getSize()) {
                            //@todo verify other things maybe
                            //file appear identical, nothing to do.
                            LOGGER("leaving alone " + path.string())
                            auto state = strategyPack.same->apply(path, attributes, manipulator);
                            deployMap.markDeployed(path, state);
                        } else {
                            LOGGER("updated file, persisting... " + path.string())
                            //file in the filesystem is newer then the one in repository
                            auto state = strategyPack.updatedInFilesystem->apply(path, attributes, manipulator);
                            deployMap.markDeployed(path, state);

                            //@todo this is mostly the same code as below, combine the two loops into a function or sth.
                            if (fs::is_directory(path) &&
                                (updateOptions.contains(UpdateOptions::followUpdatedDirectories) ||
                                 updateOptions.contains(UpdateOptions::followDirectories))) {
                                LOGGER(path.string() + " is a directory, iterating over...")
                                for (const auto& item : fs::directory_iterator(path)) {
                                    //make sure item is not in the fileMap - if it is, update will be called for it anyway... but only if called from updateAll. @todo maybe add flag to force recursive behavior? or not -- if we are assuming that it will always be called from a loop.
                                    //@todo OR, BETTER ALTERNATIVE - update could return set of paths that it updated. then it could recurse safely here, and this if(!in map) won't be needed. updateAll would just remove all already checked items from items to check list. DO IT LIKE THIS
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
                        auto state = strategyPack.newInFilesystem->apply(path, manipulator);
                        deployMap.markDeployed(path, state);

                    } else {
                        //@todo delete file
                        if (fileMap.isDeleted(path)) {
                            LOGGER("trashing " + path.string())
                            //file should be deleted, but let's check deletion time...
                            auto state = strategyPack.deletedInRepo->apply(path, manipulator);
                            deployMap.markDeployed(path, state);
                        }
                    }
                }
                //@todo this iterates over all directories and files in path, which means that it will probably call some methods twice or even more times on some files. make sure it doesn't
                if (fs::is_directory(path) && updateOptions.contains(UpdateOptions::followDirectories)) {
                    LOGGER(path.string() + " is a directory, iterating over...")
                    for (const auto& item : fs::directory_iterator(path)) {
                        //make sure item is not in the fileMap - if it is, update will be called for it anyway... but only if called from updateAll. @todo maybe add flag to force recursive behavior? or not -- if we are assuming that it will always be called from a loop.
                        //@todo same as above, this check won't be needed if update() returns set of processed files.
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
            auto& attributes = fileMap[path];
            if (attributes) {
                if (deployMap.isDeployed(path)) {
                    //file is in file map and was deployed but is not on filesystem, removing (user must have deleted it)
                    LOGGER("deleting " + path.string());
                    auto state = strategyPack.deletedInFilesystem->apply(path, attributes, manipulator);
                    deployMap.markDeployed(path, state);
                } else {
                    //file wasn't deployed. must be a new file from another node. restore.
                    //@todo this is duplicated code. move to strategies or sth. (three times as of writing this. seriously, do something about it)
                    LOGGER("not deployed " + path.string())
                    auto state = strategyPack.newInRepo->apply(path, attributes, manipulator);
                    deployMap.markDeployed(path, state);
                }
            } else {
                if (fileMap.isDeleted(path)) {
                    LOGGER("file deleted, but not in filesystem so nothing to do: " + path.string())
                    //@todo add this to strategyPack?
                    deployMap.markDeployed(path, DeployState::notDeployed);
                } else {
                    //file is neither on file system nor in the map. someone trying to update unknown file?
                    LOGGER("file unknown " + path.string());
                    //@todo add this to strategyPack?
                    //@todo add strategy for this ?
                }
            }
        }

        //    }
    }

    void Repository::syncLocalChanges() {
        //@todo process files from incoming directory here? or somewhere else?
        //@todo change the name of this function to updateAll or sth
        for (const auto& i : fileMapRenderer.renderMap(journal)) {
            LOGGER("update()" + i.first.string())
            update(i.first, localSyncPack, {UpdateOptions::followUpdatedDirectories});
        }
    }


    void Repository::deploy() {
        for (const auto& i : fileMapRenderer.renderMap(journal)) {
            //@todo add force level, by using different pack
            update(i.first, deployPack, {});
        }
    }

    Repository::Repository(const IRepository::RepoIdType& repositoryId, const std::shared_ptr<IStorage>& storage,
                           const JournalPtr& journal, const Repository::RepoDeployMap& deployMap) : Repository(repositoryId,
                                                                                                               storage) {
        this->journal = journal;
        this->deployMap = deployMap;
    }

    const IRepository::RepositoryActionStrategyPack& Repository::getDeployPack() const {
        return deployPack;
    }

    const IRepository::RepositoryActionStrategyPack& Repository::getLocalSyncPack() const {
        return localSyncPack;
    }

    const IRepository::RepositoryActionStrategyPack& Repository::getFullPack() const {
        return fullPack;
    }
    const IPathTransformer& Repository::getPathTransformer() {
        //@todo should I check if this is null?
        return *pathTransformer;
    }

    //Repository::Repository() : Repository("", nullptr) {}


    auto Repository::RepoDeployMap::begin() -> decltype(deployMap.begin()) {
        return deployMap.begin();
    }

    auto Repository::RepoDeployMap::end() -> decltype(deployMap.end()) {
        return deployMap.end();
    }

    auto
            Repository::RepoDeployMap::operator[](const fs::path& path) -> decltype(deployMap[fs::current_path()]) {
        return deployMap[path];
    }

    void Repository::RepoDeployMap::markDeployed(const fs::path& path, DeployState deployState) {
        if (deployState == DeployState::deployed) {
            LOGGER("marking " + path.string() + " as deployed")
            deployMap[path] = true;
        } else if (deployState == DeployState::notDeployed) {
            LOGGER("marking " + path.string() + " as not deployed")
            deployMap[path] = false;
        }//else unchanged
    }

    bool Repository::RepoDeployMap::DeployAttributes::isDeployed() const {
        return deployed;
    }

    void Repository::RepoDeployMap::DeployAttributes::setDeployed(bool deployed) {
        LOGGER("setDirect deployed")
        DeployAttributes::deployed = deployed;
    }

    Repository::RepoDeployMap::DeployAttributes::DeployAttributes(bool deployed) : deployed(deployed) {}

    Repository::RepoDeployMap::DeployAttributes::DeployAttributes() = default;

    //@todo maybe move actual implementation code from methods like repository.persist, delete, trash... etc. to those strategies? just a thought

    class StrategyPersist : public RepositoryActionStrategy {
        DeployState
        apply(const fs::path& path, const std::optional<RepositoryAttributes>& attributes, RepositoryManipulator& manipulator) override {
            manipulator.persist(path, attributes);
            return DeployState::deployed;//it was UNCHANGED, but file is clearly in the filesystem, so it's deployed!
            //UNCHANGED made some sense, because persisting the file doesn't actually deploy it, but new files weren't marked as deployed, so update thought they were deleted. and new files go through persist, so it should return deployed.
        }
    };

    class StrategyRestore : public RepositoryActionStrategy {
    public:
        DeployState
        apply(const fs::path& path, const std::optional<RepositoryAttributes>& attributes, RepositoryManipulator& manipulator) override {
            LOGGER("restoring "+path.string() )
            manipulator.restoreFileFromStorage(path, attributes);
            manipulator.restoreAttributes(path, attributes);
            return DeployState::deployed;
        }
    };

    class StrategyTrash : public RepositoryActionStrategy {
    public:
        DeployState
        apply(const fs::path& path, const std::optional<RepositoryAttributes>& attributes, RepositoryManipulator& manipulator) override {
            manipulator.trash(path);
            return DeployState::notDeployed;
        }
    };

    class StrategyRemove : public RepositoryActionStrategy {
    public:
        DeployState
        apply(const fs::path& path, const std::optional<RepositoryAttributes>& attributes, RepositoryManipulator& manipulator) override {
            manipulator.remove(path, attributes);
            return DeployState::notDeployed;
        }
    };

    class StrategyDelete : public RepositoryActionStrategy {
    public:
        DeployState
        apply(const fs::path& path, const std::optional<RepositoryAttributes>& attributes, RepositoryManipulator& manipulator) override {
            //@todo implement
            return DeployState::notDeployed;
        }
    };

    class StrategyNull : public RepositoryActionStrategy {
    public:
        DeployState
        apply(const fs::path& path, const std::optional<RepositoryAttributes>& attributes, RepositoryManipulator& manipulator) override {
            LOGGER("null strategy")
            return DeployState::unchanged;
        }
    };

    std::shared_ptr<RepositoryActionStrategy>
    Repository::RepositoryActionStrategyFactory::create(RepositoryAction action) const {
        switch (action) {
            case RepositoryAction::persist:
            case RepositoryAction::update:
                //update and persist are the same
                return std::make_shared<StrategyPersist>();
            case RepositoryAction::erase:
                return std::make_shared<StrategyDelete>();
            case RepositoryAction::trash:
                return std::make_shared<StrategyTrash>();
            case RepositoryAction::remove:
                return std::make_shared<StrategyRemove>();
            case RepositoryAction::restore:
                return std::make_shared<StrategyRestore>();
            default://<---- this is here just so that compiler won't complain about return reaching end of function.
            case RepositoryAction::nop:
                return std::make_shared<StrategyNull>();
        }
    }


    Repository::RepositoryActionStrategyPack
    Repository::RepositoryActionStrategyFactory::createPack(Repository::RepoActionPack actionPack) const {
        RepositoryActionStrategyPack strategyPack;
        strategyPack.updatedInRepo = create(actionPack.updatedInRepo);
        strategyPack.updatedInFilesystem = create(actionPack.updatedInFilesystem);
        strategyPack.same = create(actionPack.same);
        strategyPack.newInRepo = create(actionPack.newInRepo);
        strategyPack.newInFilesystem = create(actionPack.newInFilesystem);
        strategyPack.deletedInRepo = create(actionPack.deletedInRepo);
        strategyPack.deletedInFilesystem = create(actionPack.deletedInFilesystem);
        return strategyPack;
    }
}// namespace bsc