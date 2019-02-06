//
// Created by stilgar on 06.11.18.
//

#ifndef BASYCO_REPOSITORYSOURCE_H
#define BASYCO_REPOSITORYSOURCE_H


#include <p2p/logic/sources/EventQueueSource.h>
#include <repo/repository/logic/events/RepositoryEvent.h>
#include <repo/repository/IRepository.h>

class RepositorySource : public EventQueueSource<RepositoryEvent> {
public:
    explicit RepositorySource(SourceManager &sourceManager);

    void requestRepositoryInfo(IRepository::RepoIdType id);
};


#endif //BASYCO_REPOSITORYSOURCE_H
