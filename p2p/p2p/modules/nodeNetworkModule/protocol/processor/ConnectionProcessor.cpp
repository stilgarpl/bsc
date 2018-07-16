//
// Created by stilgar on 20.08.17.
//

#include <iostream>
#include <p2p/modules/nodeNetworkModule/protocol/connection/Connection.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/LogicContext.h>
#include <p2p/modules/nodeNetworkModule/protocol/logic/sources/ConnectionSource.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/ProcessorContext.h>
#include <p2p/role/Roles.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/NodeContext.h>


ConnectionProcessor::ConnectionProcessor(Connection &connection) : connection(connection) {

}

void ConnectionProcessor::run() {

    //set up context
    Context::Ptr context = connection.getConnectionContext();
    Context::setActiveContext(context);
    logger.info("ConnectionProcessor start " + context->get<NodeContext>()->getNodeInfo().getNodeId());
    auto lc = context->get<LogicContext>();
    //auto &logicManager = lc->getLogicManager();
    //auto connectionSourcePtr = logicManager.getSource<ConnectionSource>();

    auto processorContext = context->set<ProcessorContext, ConnectionProcessor &>(*this);
    Roles::setActiveScope(&connection);
    while (!this->isStopping()) {
        auto packetToProcess = connection.receive();
        if (packetFilter->filter(packetToProcess)) {
            if (packetToProcess != nullptr) {
//            LOGGER("processing packet " );
                processorContext->setThisPacket(packetToProcess);
                packetToProcess->process(context);
            } else {
                //@todo error handling
            }
        } else {
            LOGGER(std::string("packet ") + typeid(*packetToProcess).name() + "was filtered ");
            //@todo add event/error response
        }

    }

}

ConnectionProcessor::~ConnectionProcessor() {

    this->stop();

}

