//
// Created by stilgar on 21.08.17.
//

#include "ConnectionContext.h"

ConnectionContext::ConnectionContext(Connection &connection) : connection(connection) {}

Connection &ConnectionContext::getConnection() const {
    return connection;
}
