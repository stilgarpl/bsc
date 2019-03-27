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
#include <p2p/logic/evaluators/CommonEvaluators.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/actions/NetworkActions.h>
#include <repo/repository/network/RepoQuery.h>
#include <p2p/logic/conditions/TimeConditions.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/conditions/NetworkConditions.h>
#include <repo/repository/logic/actions/RepositoryActions.h>
#include <repo/repository/logic/evaluators/RepoEvaluators.h>
#include <p2p/logic/actions/CommonActions.h>
#include <repo/repository/network/RepoProcessors.h>
#include <p2p/logic/conditions/TriggerConditions.h>
#include <p2p/logic/sources/TriggerSource.h>
#include <p2p/logic/chain/ChainEvaluators.h>
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

    //@todo implements all those required methods.
//    when(TimeConditions::every(15s)).fireNewGenericAction(LOGGER("super secret generic action");


//    when(TimeConditions::every(15s)).newChain("lockTest").lockChain().fireNewGenericChainAction([]{LOGGER("LOCK TEST secret generic action");})
//            .fireNewGenericChainAction([]{std::this_thread::sleep_for(18s);LOGGER("LOCK TEST super 2 action ");})
//            .fireNewGenericChainAction([]{LOGGER("LOCK TEST super 2 action ");}).unlockChain();

    when(TimeConditions::every(60s))
            .fireNewGenericAction([this] {
                node.getLogicManager().getSource<TriggerSource>()->fireTrigger<std::string>("updateRepoTrigger");
            });
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
            .lockChain()
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
            .unlockChain();
    stage1.elseChain()
            .fireNewGenericChainAction([]() { LOGGER("else chain!"); })
            .unlockChain();
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

RepoModule::RepoModule(INode &node) : NodeModuleConfigDependent(node) {
    setRequired<BasicModule>();
}

void RepoModule::printHistory() {
    LOGGER("printing history")
    selectedRepository->getJournal()->printHistory();
    LOGGER("history printed")
}

void RepoModule::loadRepository(const Repository::RepoIdType &repoId) {
    RepositoryPtr ptr = std::make_shared<Repository>(repoId);

    repositoryManager.addRepository(ptr);

//@todo throw exception if repo does not exist
    ptr->setJournal(node.getConfigurationManager().loadData<JournalPtr>(configuration().getRepositoryDataPath() / (repoId + ".xml")));
//    {
//        std::ifstream is(path);
//        cereal::XMLInputArchive ia(is);
//        ia >> ptr->getJournal();
//    }
    LOGGER(ptr->getJournal()->getChecksum());
}

void RepoModule::saveRepository(const Repository::RepoIdType &repoId) {
    auto rep = findRepository(repoId);
//    rep->getJournal()->commitState();
    rep->commit();
    fs::path savePath = fs::temp_directory_path() / (repoId + ".xml");
    node.getConfigurationManager().saveData<JournalPtr>(configuration().getRepositoryDataPath() / (repoId + ".xml"), rep->getJournal());
//    {
//        std::ofstream os(savePath);
//        cereal::XMLOutputArchive oa(os);
//        oa << rep->getJournal();
//    }

}

RepositoryPtr RepoModule::createRepository(const Repository::RepoIdType &repoId) {
    RepositoryPtr ptr = std::make_shared<Repository>(repoId);
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

    auto netModule = node.getModule<NodeNetworkModule>();
    auto localRepo = findRepository(repoId);
    LOGGER("downloading repo")
    JournalGroup::Request::Ptr req = JournalGroup::Request::getNew<Status::REQUEST>();
    req->setRepoId(repoId);
    JournalGroup::Response::Ptr res = netModule->sendPacketToNode(remoteId, req);
    if (res != nullptr) {
        LOGGER("response received")
        auto remoteJournal = res->getJournal();
        LOGGER("received journal with checksum " + res->getJournal()->getChecksum())
        if (localRepo != nullptr) {
            localRepo->getJournal()->merge(remoteJournal);
        } else {
            localRepo = createRepository(repoId);
            localRepo->setJournal(remoteJournal);

        }
//        localRepo->buildFileMap();
        localRepo->downloadStorage();
    } else {
        LOGGER("no response")
    }


}

void RepoModule::updateFile(const fs::path &path) {

    if (selectedRepository != nullptr) {
        selectedRepository->update(path);

    }

}

void RepoModule::updateAllFiles() {

    if (selectedRepository != nullptr) {
        selectedRepository->update();

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

    auto &networkSub = getSubModule<NodeNetworkModule>();
    networkSub.registerPacketProcessor<RepoQuery>(RepoProcessors::queryProcessor);

}

void RepoModule::downloadRepository(const Repository::RepoIdType &repoId) {
    //@todo error handling
    findRepository(repoId)->downloadStorage();
}

const std::filesystem::path &RepoModuleConfiguration::getRepositoryDataPath() const {
    return repositoryDataPath;
}

void RepoModuleConfiguration::setRepositoryDataPath(const std::filesystem::path &repositoryDataPath) {
    RepoModuleConfiguration::repositoryDataPath = repositoryDataPath;
}
