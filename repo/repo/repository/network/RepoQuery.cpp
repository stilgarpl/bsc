//
// Created by stilgar on 05.02.19.
//

#include <logic/context/LogicContext.h>
#include "RepoQuery.h"



const IRepository::RepoIdType &RepoQuery::Request::getRepoId() const {
    return repoId;
}

void RepoQuery::Request::setRepoId(const IRepository::RepoIdType &repoId) {
    Request::repoId = repoId;
}
