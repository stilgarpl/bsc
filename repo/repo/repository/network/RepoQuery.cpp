//
// Created by stilgar on 05.02.19.
//

#include <p2p/node/context/LogicContext.h>
#include <repo/repository/logic/sources/RepositorySource.h>
#include "RepoQuery.h"

void RepoQuery::Request::process(Context::Ptr context) {
    BasePacket::process(context);
    LOGGER("processing repo request")
//
//    auto lc = context->get<LogicContext>();
//    if (lc != nullptr) {
//        auto repoSource = lc->getLogicManager().getSource<RepositorySource>();
//        repoSource->requestRepositoryInfo(repoId);
//
//    }

}

const IRepository::RepoIdType &RepoQuery::Request::getRepoId() const {
    return repoId;
}

void RepoQuery::Request::setRepoId(const IRepository::RepoIdType &repoId) {
    Request::repoId = repoId;
}
