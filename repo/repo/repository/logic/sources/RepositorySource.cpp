//
// Created by stilgar on 06.11.18.
//

#include "RepositorySource.h"

RepositorySource::RepositorySource(SourceManager &sourceManager) : EventQueueSource(sourceManager) {

}

void RepositorySource::requestRepositoryInfo(IRepository::RepoIdType id) {
    auto event = newEvent();
//        event->setRequestId(requestId);
    queueEvent(event);
}
