//
// Created by stilgar on 05.11.17.
//

#include "RepoModule.h"
#include <core/factory/FactoryContext.h>
#include <logic/actions/CommonActions.h>
#include <logic/chain/ChainEvaluators.h>
#include <logic/conditions/TimeConditions.h>
#include <logic/conditions/TriggerConditions.h>
#include <logic/evaluators/CommonEvaluators.h>
#include <logic/evaluators/TriggerEvaluators.h>
#include <logic/sources/TriggerSource.h>
#include <p2p/modules/basic/BasicModule.h>
#include <p2p/modules/command/CommandModule.h>
#include <p2p/modules/network/protocol/logic/actions/NetworkActions.h>
#include <p2p/modules/network/protocol/logic/conditions/NetworkConditions.h>
#include <repo/repository/Repository.h>
#include <repo/repository/logic/actions/RepositoryActions.h>
#include <repo/repository/logic/evaluators/RepoEvaluators.h>
#include <repo/repository/network/RepoProcessors.h>
#include <repo/repository/network/RepoQuery.h>
#include <repo/repository/storage/ManagedStorageFactory.h>
#include <repo/repository/storage/StorageFactory.h>
#include <repo/repository/storage/StorageFactorySpecialization.h>
#include <repo/repository/transformer/PathTransformerRuleFactory.h>


namespace bsc {
    void RepoModule::setupActions(ILogicModule::SetupActionHelper& actionHelper) {
    }

    bool RepoModule::assignActions(ILogicModule::AssignActionHelper& actionHelper) {
        bool ret = true;

        auto updateChainGroup = chainGroup("updateChain");

        if (configuration().isAutoProcess()) {
            when(TimeConditions::every(60s))
                    .fireNewGenericAction([this] {
                        node.getLogicManager().getSource<TriggerSource>()->fireTrigger<std::string>(
                                "updateRepoTrigger");
                    });
            when(TimeConditions::every(60s))
                    .fireNewGenericAction([this] {
                        for (const auto& repository : repositoryManager.getRepositories()) {
                            node.getLogicManager().getSource<TriggerSource>()->fireTrigger<std::string, std::string>(
                                    "syncLocalRepo", repository->getRepositoryId());
                        }
                    });
        }
        when(TriggerConditions::trigger<std::string>("updateRepoTrigger"))
                .fireNewGenericAction(
                        CommonActions::foreachActionGetter(
                                NetworkActions::broadcastPacket,
                                [this] { return this->repositoryManager.getRepositories(); },
                                [](RepositoryPtr rep) -> BasePacketPtr {
                                    auto p = RepoQuery::Request::getNew();
                                    p->setRepoId(rep->getRepositoryId());
                                    return p;
                                }),
                        CommonEvaluators::foreachValue<BasePacketPtr>());

        auto start = when(NetworkConditions::packetReceived<RepoQuery::Response>())
                             .newChain("repoUpdateChain");
        auto stage1 = start
                              //@todo add lock name function
                              .lockChain(LockConfiguration::eval(CommonEvaluators::stack(
                                      RepoEvaluators::getRepoId,
                                      ChainEvaluators::chainResult(start))))
                              .ifTrue(RepositoryActions::checkIfUpdateRequired,
                                      RepoEvaluators::currentJournalFromRepoQueryResponse,
                                      RepoEvaluators::newJournalFromRepoQueryResponse);
        stage1.thenChain()
                .fireNewGenericChainAction(RepositoryActions::updateJournal,
                                           CommonEvaluators::stack(
                                                   RepoEvaluators::getRepoId,
                                                   ChainEvaluators::chainResult(start)),
                                           CommonEvaluators::stack(
                                                   RepoEvaluators::newJournalFromRepoQueryResponse,
                                                   ChainEvaluators::chainResult(start)))
                .fireNewGenericChainAction(RepositoryActions::downloadRepository,
                                           CommonEvaluators::stack(
                                                   RepoEvaluators::getRepoId,
                                                   ChainEvaluators::chainResult(start)))
                .fireNewGenericChainAction(RepositoryActions::deployRepository,
                                           CommonEvaluators::stack(
                                                   RepoEvaluators::getRepoId,
                                                   ChainEvaluators::chainResult(start)))
                .unlockChain();
        stage1.elseChain()
                .fireNewGenericChainAction([]() { LOGGER("else chain!"); })
                .unlockChain();

        auto syncStart = when(TriggerConditions::trigger<std::string, std::string>("syncLocalRepo"));

        syncStart.newChain("syncRepoChain")
                //@todo add lock name function
                .lockChain(LockConfiguration::eval(CommonEvaluators::stack(
                        TriggerEvaluators::triggerValue,
                        ChainEvaluators::chainResult(syncStart))))
                .fireNewGenericChainAction(RepositoryActions::syncRepository,
                                           CommonEvaluators::stack(
                                                   TriggerEvaluators::triggerValue,
                                                   ChainEvaluators::chainResult(syncStart)))
                .fireNewGenericChainAction(RepositoryActions::saveRepository,
                                           CommonEvaluators::stack(
                                                   TriggerEvaluators::triggerValue,
                                                   ChainEvaluators::chainResult(syncStart)))
                .unlockChain();

        //    when(event<Tick>(100ms)).newChain("dupaChain").lockChain(LockConfiguration::chain()).fireNewChainAction([](auto e){
        //        LOGGER("gen1")
        //        std::this_thread::sleep_for(1s);
        //        LOGGER("gen2")
        //        return e;
        //    }).unlockChain();
        //debug
        //    stage1.lockChain()
        //            .fireNewGenericChainAction([]() { LOGGER("super secret generic action"); })
        //            .fireNewGenericChainAction([]() { LOGGER("and one after that"); })
        //            .fireNewGenericChainAction([]() { LOGGER("one more"); })
        //            .fireNewGenericChainAction([]() { LOGGER("and last one"); })
        //            .unlockChain();
        //

        return ret;
    }

    bool RepoModule::setupSources(ILogicModule::SetupSourceHelper& sourceHelper) {
        sourceHelper.requireSource<TriggerSource>();
        return true;
    }

    RepoModule::RepoModule(INode& node) : NodeModuleDependent(node, "repo") {
        setRequired<BasicModule>();
    }

    void RepoModule::printHistory() {
        LOGGER("printing history")
        selectedRepository->getJournal()->printHistory();
        LOGGER("history printed")
    }

    void RepoModule::loadRepository(const IRepository::RepoIdType& repoId) {
        //@todo get storage that this actual repository uses
        //    RepositoryPtr ptr = std::make_shared<Repository>(repoId, storageManager.getDefaultStorage());


        //@todo this method should be moved somewhere else. Journal is not the only thing of repo that needs to be saved. Repository should have serialize and RepoManager should handle loading and saving.
        //@todo throw exception if repo does not exist
        auto ptr = node.getConfigurationManager().loadData<RepositoryPtr>(
                configuration().getRepositoryDataPath() / (repoId + ".xml"));
        //    {
        //        std::ifstream is(path);
        //        cereal::XMLInputArchive ia(is);
        //        ia >> ptr->getJournal();
        //    }
        repositoryManager.addRepository(ptr);
        LOGGER(ptr->getJournal()->getChecksum());
    }

    void RepoModule::saveRepository(const IRepository::RepoIdType& repoId) {
        auto rep = findRepository(repoId);
        //    rep->getJournal()->commitState();
        rep->commit();
        auto repoPath = (configuration().getRepositoryDataPath() / (repoId + ".xml")).string();
        //    node.getConfigurationManager().saveData<JournalPtr>(configuration().getRepositoryDataPath() / (repoId + ".xml"),
        //                                                        rep->getJournal());
        node.getConfigurationManager().saveData<RepositoryPtr>(
                configuration().getRepositoryDataPath() / (repoId + ".xml"),
                rep);
        LOGGER("saving repository journal for repo " + repoId + " and checksum is " + rep->getJournal()->getChecksum())
        //    {
        //        std::ofstream os(savePath);
        //        cereal::XMLOutputArchive oa(os);
        //        oa << rep->getJournal();
        //    }
    }

    RepositoryPtr RepoModule::createRepository(const IRepository::RepoIdType& repoId) {
        //@todo use factory so RepoModule don't have to rely on Repository?
        auto ptr = std::make_shared<Repository>(repoId, storageManager.getDefaultStorage());
        repositoryManager.addRepository(ptr);
        return ptr;
    }

    void RepoModule::selectRepository(const IRepository::RepoIdType& repoId) {
        selectedRepository = findRepository(repoId);
        if (selectedRepository == nullptr) {
            LOGGER("SELECTED NULL REPOSITORY!")
        }
    }

    RepositoryPtr RepoModule::findRepository(const IRepository::RepoIdType& repoId) {
        return repositoryManager.getRepository(repoId);
    }

    const RepositoryPtr& RepoModule::getSelectedRepository() const {
        return selectedRepository;
    }

    void RepoModule::restoreRepository(const IRepository::RepoIdType& repoId) {
        repositoryManager.getRepository(repoId)->restoreAll();
    }

    void RepoModule::downloadRemoteRepository(const NodeIdType& remoteId, const IRepository::RepoIdType& repoId) {

        auto netModule = node.getModule<NetworkModule>();
        auto localRepo = findRepository(repoId);
        LOGGER("downloading repo")
        //@todo Repository is serializable. maybe request whole repository instead of just a journal? and then pass it to RepositoryManager to handle merging?
        auto req = JournalGroup::Request::getNew<Status::request>();
        req->setRepoId(repoId);
        auto res = netModule->sendPacketToNode(remoteId, req);
        if (res != nullptr) {
            LOGGER("response received")
            auto remoteJournal = res->getJournal();
            LOGGER("received journal with checksum " + res->getJournal()->getChecksum())
            if (localRepo != nullptr) {
                LOGGER("merging journal")
                //@todo this should not operate on journal directly! thread safety and other issues. journal should be hidden behind Repository.
                localRepo->getJournal()->merge(remoteJournal);
            } else {
                LOGGER("Creating new repo")
                localRepo = createRepository(repoId);
                //@todo definitely get rid of external setting of journal. Journal is internal property of Repository
                localRepo->setJournal(remoteJournal);
            }
            //        localRepo->buildFileMap();
            LOGGER("downloading storage")
            localRepo->downloadStorage();
            LOGGER("storage downloaded")
        } else {
            //@todo throw exception maybe
            LOGGER("no response")
        }
    }

    void RepoModule::updateFile(const fs::path& path) {

        if (selectedRepository != nullptr) {
            selectedRepository->update(path, selectedRepository->getFullPack(),
                                       {Repository::UpdateOptions::followDirectories});
        }
    }

    void RepoModule::updateAllFiles() {
        LOGGER("updateAllFiles")
        if (selectedRepository != nullptr) {
            LOGGER("syncing repo ..." + selectedRepository->getRepositoryId())
            selectedRepository->syncLocalChanges();
        }
    }

    void RepoModule::deployAllFiles() {

        if (selectedRepository != nullptr) {
            selectedRepository->deploy();
        }
    }


    void RepoModule::prepareSubmodules() {
        auto& commandSub = getSubModule<CommandModule>();
        commandSub.mapCommand("turbo", &RepoModule::selectRepository);

        auto& networkSub = getSubModule<NetworkModule>();
        networkSub.registerPacketProcessor<RepoQuery>(RepoProcessors::queryProcessor);
        networkSub.registerPacketProcessor<JournalGroup>([this](JournalGroup::Request::Ptr request) {
            JournalGroup::Response::Ptr response = JournalGroup::Response::getNew();

            // JournalGroup::Response* response;
            response->setRepoId(request->getRepoId());
            LOGGER("requested repo " + request->getRepoId());

            //@todo add way more error handling to the getting of the module that's not there or repo that's not there...

            auto repository = this->findRepository(request->getRepoId());
            LOGGER("journal requested for repo: " + request->getRepoId() + " and journal checksum is " +
                   repository->getJournal()->getChecksum())
            response->setJournal(repository->getJournal());


            return response;
        });
    }

    void RepoModule::downloadRepository(const IRepository::RepoIdType& repoId) {
        //@todo error handling
        findRepository(repoId)->downloadStorage();
    }

    void RepoModule::deployRepository(const IRepository::RepoIdType& repoId) {
        //@todo add to the deployed repository list?
        findRepository(repoId)->deploy();
    }

    IStoragePtr RepoModule::findStorage(const IStorage::StorageId& storageId) {
        return storageManager.findStorage(storageId);
    }

    void RepoModule::initialize() {
        auto factoryContext = node.getContext()->get<FactoryContext>();
        using namespace bsc;
        auto storageFactoryPtr = factoryContext->addFactory<StorageFactory>(getConfigurationManager().getFullDataPath(configuration().getStoragePath()));
        auto managedStorageFactoryPtr = factoryContext->addFactory<ManagedStorageFactory>(storageManager);
        const std::string defaultStorageId = "default";
        auto defaultStorage = storageFactoryPtr->create(defaultStorageId, defaultStorageId);
        storageManager.setDefaultStorage(defaultStorageId, defaultStorage);

        factoryContext->addFactory<PathTransformerRuleFactory>();
    }

    IStoragePtr
    RepoModule::createStorage(const Factory<IStoragePtr, bsc::StorageFactoryByType>::SelectorType& storageType,
                              const IStorage::StorageId& storageId) {
        auto factoryContext = node.getContext()->get<FactoryContext>();
        //@todo what if factory is null or created storage is null because storageType is wrong?
        return factoryContext->getFactory<IStoragePtr, bsc::StorageFactoryByType>()->create(storageType, storageId);
    }

    std::filesystem::path RepoModule::Configuration::getRepositoryDataPath() const {
        return repositoryDataPath;
    }

    void RepoModule::Configuration::setRepositoryDataPath(const std::filesystem::path& repositoryDataPath) {
        RepoModule::Configuration::repositoryDataPath = repositoryDataPath.string();
    }

    bool RepoModule::Configuration::isAutoProcess() const {
        return autoProcess;
    }

    void RepoModule::Configuration::setAutoProcess(bool autoProcess) {
        Configuration::autoProcess = autoProcess;
    }

    std::filesystem::path RepoModule::Configuration::getStoragePath() const {
        return storagePath;
    }

    void RepoModule::Configuration::setStoragePath(const PathType& storagePath) {
        Configuration::storagePath = storagePath;
    }
}// namespace bsc