//
// Created by stilgar on 27.08.17.
//

#ifndef BASYCO_AUTHSOURCE_H
#define BASYCO_AUTHSOURCE_H


#include <p2p/modules/nodeNetworkModule/protocol/logic/events/AuthEvent.h>
#include "p2p/logic/ISource.h"
#include "p2p/logic/sources/EventQueueSource.h"


class AuthSource : public EventQueueSource<AuthEvent> {
public:

    AuthSource(SourceManager &sourceManager);

    void auth(std::string login, std::string password);
};


#endif //BASYCO_AUTHSOURCE_H
