//
// Created by stilgar on 27.08.17.
//

#ifndef BASYCO_NETEVENT_H
#define BASYCO_NETEVENT_H

#include "logic/IEvent.h"

enum class ENetEvent {
    CONNECTION_ESTABLISHED,
    CONNECTION_ACCEPTED,
    CONNECTION_CLOSED, //as client, when closing connection
    CONNECTION_DISCONNECTED, //as server, when client is disconnected
};

class NetEvent : public IEvent<ENetEvent> {


};


#endif //BASYCO_NETEVENT_H
