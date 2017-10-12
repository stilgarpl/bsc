//
// Created by stilgar on 29.08.17.
//

#include "ConnectionEvent.h"

Connection *ConnectionEvent::getConnection() const {
    return connection;
}

void ConnectionEvent::setConnection(Connection *connection) {
    ConnectionEvent::connection = connection;
}
