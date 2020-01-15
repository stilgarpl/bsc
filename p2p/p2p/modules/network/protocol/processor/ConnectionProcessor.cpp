//
// Created by stilgar on 20.08.17.
//

#include <iostream>
#include <p2p/modules/network/protocol/connection/Connection.h>
#include <logic/context/LogicContext.h>
#include <p2p/modules/network/protocol/logic/sources/ConnectionSource.h>
#include <p2p/role/Roles.h>
#include <p2p/node/context/NodeContext.h>



ConnectionProcessor::ConnectionProcessor(Connection &connection) : connection(connection) {

}

void ConnectionProcessor::run() {

    //setDirect up context
    bsc::Context::Ptr context = connection.getConnectionContext();
    bsc::Context::setActiveContext(context);
    logger.info("ConnectionProcessor start " + context->get<NodeContext>().getNodeInfo().getNodeId());
    auto& lc = context->get<bsc::LogicContext>();
    auto& logicManager = lc.getLogicManager();
    auto connectionSourcePtr = logicManager.getSource<ConnectionSource>();

    Roles::setActiveScope(&connection);
    while (!this->isStopping()) {
        auto packetToProcess = connection.receive();

        if (packetToProcess != nullptr) {
            if (packetFilter->filter(packetToProcess)) {
//            LOGGER("processing packet " );
                connectionSourcePtr->receivedPacket(packetToProcess, &connection);
            } else {
                LOGGER(std::string("packet ") + typeid(*packetToProcess).name() + "was filtered ");
                connectionSourcePtr->droppedPacket(packetToProcess, &connection);
            }
        } else {
            //@todo error handling
        }


    }

}
