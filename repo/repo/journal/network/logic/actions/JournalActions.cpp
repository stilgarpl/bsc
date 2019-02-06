//
// Created by stilgar on 06.11.17.
//

#include "JournalActions.h"

void JournalActions::journalRequested(const JournalRequestEvent &event) {
    auto connectionContext = Context::getActiveContext()->get<ConnectionContext>();
    Connection &connection = connectionContext->getConnection();

    JournalGroup::Response::Ptr response = JournalGroup::Response::getNew<Status::RESPONSE>(event.getRequestId());

    // JournalGroup::Response* response;
    response->setRepoId(event.getRepoId());
    LOGGER("requested repo " + event.getRepoId());

    auto nodeContext = Context::getActiveContext()->get<NodeContext>();

    //@todo add way more error handling to the getting of the module that's not there or repo that's not there...
    if (nodeContext != nullptr) {
        auto &node = nodeContext->getNode();
        auto repoModule = node.getModule<RepoModule>();
        auto repository = repoModule->findRepository(event.getRepoId());
        response->setJournal(repository->getJournal());

    } else {
        LOGGER("no node context")
    }

    connection.send(response);

}
