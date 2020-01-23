//
// Created by stilgar on 29.08.17.
//

#ifndef BSC_CONNECTIONEVENT_H
#define BSC_CONNECTIONEVENT_H

#include <p2p/modules/network/protocol/connection/Connection.h>
#include "logic/IEvent.h"


namespace bsc {
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

    class ConnectionEvent : public bsc::IEvent<ConnectionEventId> {

    private:
        Connection* connection;

    public:
        Connection* getConnection() const;

        void setConnection(Connection* connection);
    };


}


#endif //BSC_CONNECTIONEVENT_H
