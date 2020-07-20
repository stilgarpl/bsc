//
// Created by Krzysztof Tulidowicz on 26.02.2020.
//

#ifndef BSC_REPOSITORYACTION_H
#define BSC_REPOSITORYACTION_H

namespace bsc {
    enum class RepositoryAction {
        persist,
        update,//this name is a little ambigious @todo make better name
        erase,
        trash,
        remove,
        restore,
        nop,
    };
}// namespace bsc

#endif