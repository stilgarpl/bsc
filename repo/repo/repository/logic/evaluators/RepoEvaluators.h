//
// Created by stilgar on 26.02.19.
//

#ifndef BASYCO_REPOEVALUATORS_H
#define BASYCO_REPOEVALUATORS_H


#include <p2p/log/Logger.h>
#include <repo/repository/logic/events/RepositoryEvent.h>
#include <repo/repository/network/RepoQuery.h>
#include <p2p/node/context/NodeContext.h>
#include <repo/node/RepoModule.h>

struct RepoEvaluators {

    //@todo actual parameter types, this isn't auto, it's SpecificPacket Repo Query Response.
    static constexpr auto newJournalFromRepoQueryResponse = [](auto e) {
        LOGGER("unwrapping event")
        //@todo error handling

        return e.getPacket()->getJournal();
    };

    //@todo actual parameter types, this isn't auto, it's SpecificPacket Repo Query Response.
    static constexpr auto currentJournalFromRepoQueryResponse = [](auto e) {
        LOGGER("unwrapping event")
        //@todo error handling.
        return NodeContext::getNodeFromActiveContext().getModule<RepoModule>()->findRepository(
                e.getPacket()->getRepoId())->getJournal();
    };

};


#endif //BASYCO_REPOEVALUATORS_H
