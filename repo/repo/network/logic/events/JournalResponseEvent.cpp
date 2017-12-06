//
// Created by stilgar on 06.11.17.
//

#include "JournalResponseEvent.h"

const JournalPtr &JournalResponseEvent::getJournal() const {
    return journal;
}

void JournalResponseEvent::setJournal(const JournalPtr journal) {
    JournalResponseEvent::journal = journal;
}

const std::string &JournalResponseEvent::getRepoId() const {
    return repoId;
}

void JournalResponseEvent::setRepoId(const std::string &repoId) {
    JournalResponseEvent::repoId = repoId;
}
