//
// Created by stilgar on 28.02.19.
//

#ifndef BSC_REPOPROCESSORS_H
#define BSC_REPOPROCESSORS_H


#include "RepoQuery.h"

namespace bsc {
    struct RepoProcessors {

        static std::function<RepoQuery::Response::Ptr(RepoQuery::Request::Ptr)> queryProcessor;

    };
}

#endif //BSC_REPOPROCESSORS_H
