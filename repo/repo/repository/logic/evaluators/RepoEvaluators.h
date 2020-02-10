//
// Created by stilgar on 26.02.19.
//

#ifndef BSC_REPOEVALUATORS_H
#define BSC_REPOEVALUATORS_H


#include <core/log/Logger.h>
#include <repo/repository/network/RepoQuery.h>
#include <p2p/core/node/context/NodeContext.h>
#include <repo/node/RepoModule.h>

namespace bsc {


    struct RepoEvaluators {

        //@todo actual parameter types, this isn't auto, it's SpecificPacket Repo Query Response.
        static constexpr auto newJournalFromRepoQueryResponse = [](auto e) {
            LOGGER("newJournalFromRepoQueryResponse event")
            //@todo error handling

            return e.getPacket()->getJournal();
        };

        //@todo actual parameter types, this isn't auto, it's SpecificPacket Repo Query Response.
        static constexpr auto currentJournalFromRepoQueryResponse = [](auto e) {
            LOGGER("currentJournalFromRepoQueryResponse event")
            //@todo error handling.
            return bsc::NodeContext::getNodeFromActiveContext().getModule<RepoModule>()->findRepository(
                    e.getPacket()->getRepoId())->getJournal();
        };

        static constexpr auto getRepoId = [](const auto& e) {
            LOGGER("currentJournalFromRepoQueryResponse event")
            //@todo error handling.
            return e.getPacket()->getRepoId();
        };

    };
}

#endif //BSC_REPOEVALUATORS_H
