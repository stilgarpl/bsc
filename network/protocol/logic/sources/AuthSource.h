//
// Created by stilgar on 27.08.17.
//

#ifndef BASYCO_AUTHSOURCE_H
#define BASYCO_AUTHSOURCE_H


#include <network/protocol/logic/events/AuthEvent.h>
#include "logic/ISource.h"
#include "logic/sources/EventQueueSource.h"


class AuthSource : public EventQueueSource<AuthEvent> {
public:

    void auth(std::string login, std::string password);
};


#endif //BASYCO_AUTHSOURCE_H
