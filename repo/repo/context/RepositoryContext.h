//
// Created by stilgar on 20.10.17.
//

#ifndef BSC_REPOSITORYCONTEXT_H
#define BSC_REPOSITORYCONTEXT_H


#include <repo/repository/RepositoryManager.h>

namespace bsc {
    class RepositoryContext {

        RepositoryManager& repositoryManager;

    public:
        explicit RepositoryContext(RepositoryManager& repositoryManager);

    };

}
#endif //BSC_REPOSITORYCONTEXT_H
