//
// Created by stilgar on 20.08.17.
//

#include <iostream>
#include "ConnectionProcessor.h"

#include "../connection/Connection.h"
#include "../context/ConnectionContext.h"

ConnectionProcessor::ConnectionProcessor(Connection &connection) : connection(connection) {

}

void ConnectionProcessor::run() {
    logger.info("ConnectionProcessor start");
    //set up context
    ConnectionContext::Setup ctxSet(connection);
    ctxSet.setup(context);


    while (!this->isStopping()) {
        auto np = connection.receive();
        np->process(context);

    }

}

Context &ConnectionProcessor::getContext() {
    return context;
}
