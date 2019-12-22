//
// Created by stilgar on 06.11.17.
//

#include "JournalActions.h"

void JournalActions::journalRequested(const JournalRequestEvent &event) {
    auto& connectionContext = Context::getActiveContext()->get<ConnectionContext>();
    Connection& connection = connectionContext.getConnection();

    JournalGroup::Response::Ptr response = JournalGroup::Response::getNew<Status::response>(event.getRequestId());

    // JournalGroup::Response* response;
    response->setRepoId(event.getRepoId());
    LOGGER("requested repo " + event.getRepoId());

    auto& nodeContext = Context::getActiveContext()->get<NodeContext>();

    //@todo add way more error handling to the getting of the module that's not there or repo that's not there...
        auto &node = nodeContext.getNode();
        auto repoModule = node.getModule<RepoModule>();
        auto repository = repoModule->findRepository(event.getRepoId());
        LOGGER("journal requested for repo: " + event.getRepoId() + " and journal checksum is " +
               repository->getJournal()->getChecksum())
        response->setJournal(repository->getJournal());


    connection.send(response);

}
