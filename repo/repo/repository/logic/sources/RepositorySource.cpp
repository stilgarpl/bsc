//
// Created by stilgar on 06.11.18.
//

#include <p2p/modules/nodeNetworkModule/protocol/context/ProcessorContext.h>
#include "RepositorySource.h"

RepositorySource::RepositorySource(SourceManager &sourceManager) : EventQueueSource(sourceManager) {

}

void RepositorySource::requestRepositoryInfo(IRepository::RepoIdType id) {
    auto event = newEvent();
    event->setRepoId(id);
    event->setEventId(RepositoryEventId::REQUEST_REPO_INFO);
    queueEvent(event);
}
