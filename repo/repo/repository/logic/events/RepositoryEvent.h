//
// Created by stilgar on 06.11.18.
//

#ifndef BASYCO_REPOSITORYEVENT_H
#define BASYCO_REPOSITORYEVENT_H


#include <p2p/logic/IEvent.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/events/NetworkingEvent.h>
#include <repo/repository/IRepository.h>

enum class RepositoryEventId {
    REQUEST_REPO_INFO,
};

class RepositoryEvent : public NetworkingEvent<RepositoryEventId> {
private:
    IRepository::RepoIdType repoId;

public:
    const IRepository::RepoIdType &getRepoId() const;

    void setRepoId(const IRepository::RepoIdType &repoId);

};


#endif //BASYCO_REPOSITORYEVENT_H
