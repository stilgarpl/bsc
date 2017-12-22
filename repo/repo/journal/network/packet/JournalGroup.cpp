//
// Created by stilgar on 18.10.17.
//

#include "JournalGroup.h"
#include <repo/journal/network/logic/sources/JournalSource.h>
#include <p2p/network/protocol/context/LogicContext.h>

void JournalGroup::Request::process(Context &context) {
    LOGGER("journal request process")
    BasePacket::process(context);
    auto lc = context.get<LogicContext>();
    if (lc != nullptr) {
        auto journalSource = lc->getLogicManager().getSource<JournalSource>();
        journalSource->journalRequested(repoId, this->getId());

    }
}

const std::string &JournalGroup::Request::getRepoId() const {
    return repoId;
}

void JournalGroup::Request::setRepoId(const std::string &repoId) {
    Request::repoId = repoId;
}

void JournalGroup::Response::process(Context &context) {
    BasePacket::process(context);
    auto lc = context.get<LogicContext>();
    if (lc != nullptr) {
        auto journalSource = lc->getLogicManager().getSource<JournalSource>();
        journalSource->journalReceived(repoId, journal);


    }
}

const std::string &JournalGroup::Response::getRepoId() const {
    return repoId;
}

void JournalGroup::Response::setRepoId(const std::string &repoId) {
    Response::repoId = repoId;
}

const JournalPtr JournalGroup::Response::getJournal() const {
    return journal;
}

void JournalGroup::Response::setJournal(const JournalPtr journal) {
    Response::journal = journal;
}
