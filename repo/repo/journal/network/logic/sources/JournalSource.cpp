//
// Created by stilgar on 18.10.17.
//

#include "JournalSource.h"

void JournalSource::work() {
    requestSource.work();
    responseSource.work();
}


JournalSource::JournalSource(SourceManager &sourceManager) : ISource(sourceManager), requestSource(sourceManager),
                                                             responseSource(sourceManager) {}

void JournalSource::journalRequested(std::string repoId, JournalGroup::Request::IdType requestId) {
    auto event = requestSource.newEvent(requestId);
    event->setRepoId(repoId);
//        event->setRequestId(requestId);
    requestSource.queueEvent(event);
}

void JournalSource::journalReceived(std::string repoId, const JournalPtr journal) {
    //networking event requires packet id, but no one would be respoding to this, so...
    auto event = responseSource.newEvent(0);
    event->setRepoId(repoId);
    event->setJournal(journal);
    responseSource.queueEvent(event);
}
