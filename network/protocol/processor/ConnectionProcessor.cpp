//
// Created by stilgar on 20.08.17.
//

#include <iostream>
#include "ConnectionProcessor.h"

#include "../connection/Connection.h"

ConnectionProcessor::ConnectionProcessor(Connection &connection) : connection(connection) {

}

void ConnectionProcessor::run() {
    std::cout << "ConnectionProcessor start" << std::endl;

    while (!this->isStopping()) {
        std::cout << "ConnectionProcessor run" << std::endl;
        auto np = connection.receive();

    }

}
