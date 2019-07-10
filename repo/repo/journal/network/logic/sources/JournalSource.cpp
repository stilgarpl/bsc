//
// Created by stilgar on 18.10.17.
//

#include "JournalSource.h"



JournalSource::JournalSource(SourceManager &sourceManager) : ISource(sourceManager), requestSource(sourceManager),
                                                             responseSource(sourceManager) {}

void JournalSource::journalRequested(const std::string &repoId, JournalGroup::Request::IdType requestId) {
    auto event = requestSource.newEvent();
    event->setRepoId(repoId);
//        event->setRequestId(requestId);
    requestSource.queueEvent(event);
}

void JournalSource::journalReceived(const std::string &repoId, const JournalPtr &journal) {
    //networking event requires packet id, but no one would be responding to this, so...
    auto event = responseSource.newEvent();
    event->setRepoId(repoId);
    event->setJournal(journal);
    responseSource.queueEvent(event);
}

void JournalSource::run() {
    waitForStop();
}

void JournalSource::onStop() {
    requestSource.stop();
    responseSource.stop();
    requestSource.join();
    responseSource.join();
}

void JournalSource::onStart() {
    requestSource.start();
    responseSource.start();
}
