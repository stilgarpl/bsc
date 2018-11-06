//
// Created by stilgar on 06.11.18.
//

#ifndef BASYCO_REPOSITORYSOURCE_H
#define BASYCO_REPOSITORYSOURCE_H


#include <p2p/logic/sources/EventQueueSource.h>
#include <repo/repository/logic/events/RepositoryEvent.h>

class RepositorySource : EventQueueSource<RepositoryEvent> {

};


#endif //BASYCO_REPOSITORYSOURCE_H
