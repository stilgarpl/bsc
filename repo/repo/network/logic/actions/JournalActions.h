//
// Created by stilgar on 06.11.17.
//

#ifndef BASYCO_JOURNALACTIONS_H
#define BASYCO_JOURNALACTIONS_H


#include <repo/network/logic/events/JournalRequestEvent.h>
#include <repo/network/logic/events/JournalResponseEvent.h>
#include <repo/network/packet/JournalGroup.h>
#include <p2p/network/protocol/context/NodeContext.h>
#include <repo/node/RepoModule.h>

class JournalActions {

public:

    static void journalRequested(const JournalRequestEvent &event) {
        Connection *connection = event.origin();

        JournalGroup::Response::Ptr response = JournalGroup::Response::getNew<Status::RESPONSE>(event.getRequestId());

        // JournalGroup::Response* response;
        response->setRepoId(event.getRepoId());


        auto nodeContext = Context::getActiveContext().get<NodeContext>();

        ///@todo add way more error handling to the getting of the module that's not there or repo that's not there...
        if (nodeContext != nullptr) {
            auto &node = nodeContext->getNode();

            auto repoModule = node.getModule<RepoModule>();

            auto &repository = repoModule->findRepository(event.getRepoId());

            response->setJournal(repository.getJournal());

        }

        connection->send(response);

    }

    static void journalReceived(const JournalResponseEvent &event) {

        //verify journal
        //merge jorunal
        //possibly replay journal

    }

};


#endif //BASYCO_JOURNALACTIONS_H
