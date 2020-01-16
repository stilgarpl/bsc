//
// Created by stilgar on 29.08.17.
//

#include "ConnectionEvent.h"


bsc::Connection* bsc::ConnectionEvent::getConnection() const {
    return connection;
}

void bsc::ConnectionEvent::setConnection(bsc::Connection* connection) {
    ConnectionEvent::connection = connection;
}
