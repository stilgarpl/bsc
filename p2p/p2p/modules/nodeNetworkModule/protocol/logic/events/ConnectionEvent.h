//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_CONNECTIONEVENT_H
#define BASYCO_CONNECTIONEVENT_H

#include <p2p/modules/nodeNetworkModule/protocol/connection/Connection.h>
#include "logic/IEvent.h"


enum class ConnectionEventId {
    NEW_CONNECTION,
    CONNECTION_REFUSED,
    CONNECTION_CLOSED,
    CONNECTION_CLOSED_CLIENT, //client side
    CONNECTION_CLOSED_SERVER, //server side
    CONNECTION_RESET,
    BAD_ADDRESS,
    CONNECTION_ACCEPTED,//server side
    CONNECTION_ESTABLISHED, //client side

};


class ConnectionEvent : public IEvent<ConnectionEventId> {

private:
    Connection *connection;

public:
    Connection *getConnection() const;

    void setConnection(Connection *connection);
};


#endif //BASYCO_CONNECTIONEVENT_H
