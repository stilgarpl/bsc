//
// Created by stilgar on 28.02.19.
//

#ifndef BASYCO_REPOPROCESSORS_H
#define BASYCO_REPOPROCESSORS_H


#include "RepoQuery.h"

struct RepoProcessors {

    static std::function<RepoQuery::Response::Ptr(RepoQuery::Request::Ptr)> queryProcessor;

};


#endif //BASYCO_REPOPROCESSORS_H
