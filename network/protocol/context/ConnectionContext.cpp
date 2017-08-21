//
// Created by stilgar on 21.08.17.
//

#include "ConnectionContext.h"

ConnectionContext::ConnectionContext(Connection &connection) : connection(connection) {}

Connection &ConnectionContext::getConnection() const {
    return connection;
}

void ConnectionContext::Setup::setup(Context &context) {

    auto ctx = context.get<ConnectionContext>(0);
    if (ctx == nullptr) {
        context.set<ConnectionContext>(0, std::ref(connection));
    }


}

ConnectionContext::Setup::Setup(Connection &connection) : connection(connection) {}
