//
// Created by Krzysztof Tulidowicz on 26.02.19.
//

#ifndef BSC_REPOEVALUATORS_H
#define BSC_REPOEVALUATORS_H

#include <log/log/Logger.h>
#include <p2p/core/node/context/NodeContext.h>
#include <repo/node/RepoModule.h>
#include <repo/repository/network/RepoQuery.h>

namespace bsc {


    struct RepoEvaluators {

        //@todo actual parameter types, this isn't auto, it's SpecificPacket Repo Query Response.
        static constexpr auto newJournalFromRepoQueryResponse = [](auto e) {
            logger.debug("newJournalFromRepoQueryResponse event");
            //@todo error handling

            return e.getPacket()->getJournal();
        };

        //@todo actual parameter types, this isn't auto, it's SpecificPacket Repo Query Response.
        static constexpr auto currentJournalFromRepoQueryResponse = [](auto e) {
            logger.debug("currentJournalFromRepoQueryResponse event");
            //@todo error handling.
            return bsc::NodeContext::getNodeFromActiveContext().getModule<RepoModule>()->findRepository(
                    e.getPacket()->getRepoId())->getJournal();
        };

        static constexpr auto getRepoId = [](const auto& e) {
            logger.debug("currentJournalFromRepoQueryResponse event");
            //@todo error handling.
            return e.getPacket()->getRepoId();
        };

    };
}

#endif //BSC_REPOEVALUATORS_H
