//
// Created by Krzysztof Tulidowicz on 21.08.17.
//

#include "ConnectionContext.h"


bsc::ConnectionContext::ConnectionContext(bsc::Connection& connection) : connection(connection) {}

bsc::Connection& bsc::ConnectionContext::getConnection() const {
    return connection;
}
