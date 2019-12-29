//
// Created by stilgar on 18.10.17.
//

#include "JournalGroup.h"
#include <logic/context/LogicContext.h>



const std::string &JournalGroup::Request::getRepoId() const {
    return repoId;
}

void JournalGroup::Request::setRepoId(const std::string &repoId) {
    Request::repoId = repoId;
}


const std::string &JournalGroup::Response::getRepoId() const {
    return repoId;
}

void JournalGroup::Response::setRepoId(const std::string &repoId) {
    Response::repoId = repoId;
}

JournalPtr JournalGroup::Response::getJournal() const {
    return journal;
}

void JournalGroup::Response::setJournal(const JournalPtr journal) {
    Response::journal = journal;
}
