//
// Created by stilgar on 29.08.17.
//

#ifndef BASYCO_CONNECTIONEVENT_H
#define BASYCO_CONNECTIONEVENT_H

#include "../../../../logic/IEvent.h"
#include "../../connection/Connection.h"

enum class ConnectionEventId {
    NEW_CONNECTION,

};


class ConnectionEvent : public IEvent<ConnectionEventId> {

private:
    Connection *connection;

public:
    Connection *getConnection() const;

    void setConnection(Connection *connection);
};


#endif //BASYCO_CONNECTIONEVENT_H
