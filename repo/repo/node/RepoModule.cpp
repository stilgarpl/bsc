//
// Created by stilgar on 05.11.17.
//

#include <p2p/network/node/modules/BasicModule.h>
#include <repo/network/logic/events/JournalRequestEvent.h>
#include <repo/network/logic/actions/JournalActions.h>
#include <repo/network/logic/sources/JournalSource.h>
#include "RepoModule.h"

void RepoModule::setupActions(LogicManager &logicManager) {
    logicManager.setAction<JournalRequestEvent>("journalRequest", JournalActions::journalRequested);
    logicManager.setAction<JournalResponseEvent>("journalReceive", JournalActions::journalReceived);
}

bool RepoModule::assignActions(LogicManager &logicManager) {
    bool ret = logicManager.assignAction<JournalRequestEvent>("journalRequest");
    ret &= logicManager.assignAction<JournalResponseEvent>("journalReceive");
    return ret;
}

bool RepoModule::setupSources(LogicManager &logicManager) {
    logicManager.addSource<JournalSource>();
    return true;
}

RepoModule::RepoModule(INode &node) : NodeModule(node) {
    setRequired<BasicModule>();
}

void RepoModule::printHistory() {
    LOGGER("printing history")
    selectedRepository->getJournal()->printHistory();
    LOGGER("history printed")
}

void RepoModule::loadRepository(const Repository::RepoIdType &repoId, fs::path path) {
    RepositoryPtr ptr = std::make_shared<Repository>(repoId);

    repositoryManager.addRepository(ptr);
    {
        std::ifstream is(path);
        cereal::XMLInputArchive ia(is);
        ia >> ptr->getJournal();
    }
    LOGGER(ptr->getJournal()->getChecksum());
}

void RepoModule::saveRepository(const Repository::RepoIdType &repoId) {
    auto rep = findRepository(repoId);
    rep->getJournal()->commitState();

    fs::path savePath = fs::temp_directory_path() / (repoId + ".xml");

    {
        std::ofstream os(savePath);
        cereal::XMLOutputArchive oa(os);
        oa << rep->getJournal();
    }

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
        selectedRepository->persist(path);//getJournal()->append(JournalMethod::ADDED, path);

    }
}

RepositoryPtr RepoModule::findRepository(const Repository::RepoIdType &repoId) {
    return repositoryManager.getRepository(repoId);
}

const RepositoryPtr &RepoModule::getSelectedRepository() const {
    return selectedRepository;
}
