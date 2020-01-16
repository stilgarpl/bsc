//
// Created by stilgar on 20.10.17.
//

#ifndef BASYCO_REPOSITORYCONTEXT_H
#define BASYCO_REPOSITORYCONTEXT_H


#include <repo/repository/RepositoryManager.h>
namespace bsc {
    class RepositoryContext {

        RepositoryManager& repositoryManager;

    public:
        explicit RepositoryContext(RepositoryManager& repositoryManager);

    };

}
#endif //BASYCO_REPOSITORYCONTEXT_H
