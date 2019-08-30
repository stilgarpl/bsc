//
// Created by stilgar on 05.11.17.
//

#include <p2p/modules/basic/BasicModule.h>
#include <repo/journal/network/logic/events/JournalRequestEvent.h>
#include <repo/journal/network/logic/actions/JournalActions.h>
#include <repo/journal/network/logic/sources/JournalSource.h>
#include <repo/repository/storage/network/logic/sources/StorageSource.h>
#include <repo/repository/storage/network/logic/actions/StorageActions.h>
#include <p2p/modules/command/CommandModule.h>
#include <logic/evaluators/CommonEvaluators.h>
#include <p2p/modules/network/protocol/logic/actions/NetworkActions.h>
#include <repo/repository/network/RepoQuery.h>
#include <logic/conditions/TimeConditions.h>
#include <p2p/modules/network/protocol/logic/conditions/NetworkConditions.h>
#include <repo/repository/logic/actions/RepositoryActions.h>
#include <repo/repository/logic/evaluators/RepoEvaluators.h>
#include <logic/actions/CommonActions.h>
#include <repo/repository/network/RepoProcessors.h>
#include <logic/conditions/TriggerConditions.h>
#include <logic/sources/TriggerSource.h>
#include <logic/chain/ChainEvaluators.h>
#include <logic/evaluators/TriggerEvaluators.h>
#include <core/factory/FactoryContext.h>
#include <repo/repository/storage/StorageFactory.h>
#include <repo/repository/storage/StorageFactorySpecialization.h>
#include <repo/repository/storage/ManagedStorageFactory.h>
#include "RepoModule.h"

//const fs::path RepoModule::repositoryDataPath = fs::path("repository");


void RepoModule::setupActions(ILogicModule::SetupActionHelper &actionHelper) {
    actionHelper.setAction<JournalRequestEvent>("journalRequest", JournalActions::journalRequested);
    actionHelper.setAction<JournalResponseEvent>("journalReceive", JournalActions::journalReceived);
    actionHelper.setAction<StorageResourceRequestEvent>("storageQuery", StorageActions::resourceRequested);
}

bool RepoModule::assignActions(ILogicModule::AssignActionHelper &actionHelper) {
    bool ret = actionHelper.assignAction<JournalRequestEvent>("journalRequest");
    ret &= actionHelper.assignAction<JournalResponseEvent>("journalReceive");
    ret &= actionHelper.assignAction<StorageResourceRequestEvent>("storageQuery");

    auto updateChainGroup = chainGroup("updateChain");

    if (configuration().isAutoProcess()) {
        when(TimeConditions::every(60s))
                .fireNewGenericAction([this] {
                    node.getLogicManager().getSource<TriggerSource>()->fireTrigger<std::string>("updateRepoTrigger");
                });
        when(TimeConditions::every(60s))
                .fireNewGenericAction([this] {
                    for (const auto &repository : repositoryManager.getRepositories()) {
                        node.getLogicManager().getSource<TriggerSource>()->fireTrigger<std::string, std::string>(
                                "syncLocalRepo", repository->getRepositoryId());
                    }

                });
    }
    when(TriggerConditions::trigger<std::string>("updateRepoTrigger")).fireNewGenericAction(
            CommonActions::foreachActionGetter(NetworkActions::broadcastPacket,
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

bool RepoModule::setupSources(ILogicModule::SetupSourceHelper &sourceHelper) {
    sourceHelper.requireSource<JournalSource>();
    sourceHelper.requireSource<StorageSource>();
    sourceHelper.requireSource<TriggerSource>();
    return true;
}

RepoModule::RepoModule(INode &node) : NodeModuleDependent(node, "repo") {
    setRequired<BasicModule>();
}

void RepoModule::printHistory() {
    LOGGER("printing history")
    selectedRepository->getJournal()->printHistory();
    LOGGER("history printed")
}

void RepoModule::loadRepository(const Repository::RepoIdType &repoId) {
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

void RepoModule::saveRepository(const Repository::RepoIdType &repoId) {
    auto rep = findRepository(repoId);
//    rep->getJournal()->commitState();
    rep->commit();
    auto repoPath = (configuration().getRepositoryDataPath() / (repoId + ".xml")).string();
//    node.getConfigurationManager().saveData<JournalPtr>(configuration().getRepositoryDataPath() / (repoId + ".xml"),
//                                                        rep->getJournal());
    node.getConfigurationManager().saveData<RepositoryPtr>(configuration().getRepositoryDataPath() / (repoId + ".xml"),
                                                           rep);
    LOGGER("saving repository journal for repo " + repoId + " and checksum is " + rep->getJournal()->getChecksum())
//    {
//        std::ofstream os(savePath);
//        cereal::XMLOutputArchive oa(os);
//        oa << rep->getJournal();
//    }

}

RepositoryPtr RepoModule::createRepository(const Repository::RepoIdType &repoId) {
    auto ptr = std::make_shared<Repository>(repoId, storageManager.getDefaultStorage());
    repositoryManager.addRepository(ptr);
    return ptr;

}

void RepoModule::selectRepository(const Repository::RepoIdType &repoId) {
    selectedRepository = findRepository(repoId);
    if (selectedRepository == nullptr) {
        LOGGER("SELECTED NULL REPOSITORY!")
    }

}

void RepoModule::persistFile(const fs::path &path) {
    if (selectedRepository != nullptr) {
        selectedRepository->persist(path);//getJournal()->append(JournalMethod::ADDED,JournalTarget::FILE, path);

    }
}

RepositoryPtr RepoModule::findRepository(const Repository::RepoIdType &repoId) {
    return repositoryManager.getRepository(repoId);
}

const RepositoryPtr &RepoModule::getSelectedRepository() const {
    return selectedRepository;
}

void RepoModule::restoreRepository(const Repository::RepoIdType &repoId) {
    repositoryManager.getRepository(repoId)->restoreAll();
}

void RepoModule::downloadRemoteRepository(const NodeIdType &remoteId, const Repository::RepoIdType &repoId) {

    auto netModule = node.getModule<NetworkModule>();
    auto localRepo = findRepository(repoId);
    LOGGER("downloading repo")
    auto req = JournalGroup::Request::getNew<Status::REQUEST>();
    req->setRepoId(repoId);
    auto res = netModule->sendPacketToNode(remoteId, req);
    if (res != nullptr) {
        LOGGER("response received")
        auto remoteJournal = res->getJournal();
        LOGGER("received journal with checksum " + res->getJournal()->getChecksum())
        if (localRepo != nullptr) {
            LOGGER("merging journal")
            localRepo->getJournal()->merge(remoteJournal);
        } else {
            LOGGER("Creating new repo")
            localRepo = createRepository(repoId);
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

void RepoModule::updateFile(const fs::path &path) {

    if (selectedRepository != nullptr) {
        selectedRepository->update(path, selectedRepository->getFullPack(), {Repository::UpdateOptions::FOLLOW_DIRECTORIES});

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

void RepoModule::ignoreFile(const fs::path &path) {
    if (selectedRepository != nullptr) {
        selectedRepository->ignore(path);

    }
}

void RepoModule::persistFile(const Repository::RepoIdType &repoId, const fs::path &path) {
    //@todo error handling
    findRepository(repoId)->persist(path);
}

void RepoModule::prepareSubmodules() {
    auto &commandSub = getSubModule<CommandModule>();
    commandSub.mapCommand("turbo", &RepoModule::selectRepository);

    auto &networkSub = getSubModule<NetworkModule>();
    networkSub.registerPacketProcessor<RepoQuery>(RepoProcessors::queryProcessor);

}

void RepoModule::downloadRepository(const Repository::RepoIdType &repoId) {
    //@todo error handling
    findRepository(repoId)->downloadStorage();
}

void RepoModule::deployRepository(const Repository::RepoIdType &repoId) {
    //@todo add to the deployed repository list?
    findRepository(repoId)->deploy();
}

IStoragePtr RepoModule::findStorage(const IStorage::StorageId &storageId) {
    return storageManager.findStorage(storageId);
}

void RepoModule::initialize() {
    auto factoryContext = node.getContext()->get<FactoryContext>();
    FactoryPtr<IStoragePtr,StorageFactoryByType> storageFactoryPtr = std::make_shared<StorageFactory>();
    FactoryPtr<IStoragePtr,StorageFactoryByName> managedStorageFactoryPtr = std::make_shared<ManagedStorageFactory>();
    factoryContext->setFactory<IStoragePtr,StorageFactoryByType>(storageFactoryPtr);
    factoryContext->setFactory<IStoragePtr,StorageFactoryByName>(managedStorageFactoryPtr);
}

std::filesystem::path RepoModule::Configuration::getRepositoryDataPath() const {
    return repositoryDataPath;
}

void RepoModule::Configuration::setRepositoryDataPath(const std::filesystem::path &repositoryDataPath) {
    RepoModule::Configuration::repositoryDataPath = repositoryDataPath;
}

bool RepoModule::Configuration::isAutoProcess() const {
    return autoProcess;
}

void RepoModule::Configuration::setAutoProcess(bool autoProcess) {
    Configuration::autoProcess = autoProcess;
}
