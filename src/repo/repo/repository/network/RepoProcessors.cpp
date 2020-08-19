//
// Created by Krzysztof Tulidowicz on 28.02.19.
//

#include "RepoProcessors.h"



#include <core/log/Logger.h>
#include <p2p/core/node/context/NodeContext.h>
#include <repo/node/RepoModule.h>
namespace bsc {

    std::function<RepoQuery::Response::Ptr(RepoQuery::Request::Ptr)> RepoProcessors::queryProcessor = [](
            const RepoQuery::Request::Ptr& ptr) {
        LOGGER("Repo Query Processor")
        auto repoMod = bsc::NodeContext::getNodeFromActiveContext().getModule<RepoModule>();
        auto repo = repoMod->findRepository(ptr->getRepoId());
        RepoQuery::Response::Ptr res = RepoQuery::Response::getNew();
        res->setRepoId(ptr->getRepoId());
        if (repo != nullptr) {
            LOGGER("repo isn't null")
            res->setExists(true);
            res->setJournal(repo->getJournal());
        } else {
            res->setExists(false);
            LOGGER("repo is null")
        }
        return res;
    };
}