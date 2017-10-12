//
// Created by stilgar on 20.08.17.
//

#include <iostream>
#include <network/protocol/connection/Connection.h>
#include <network/protocol/context/LogicContext.h>
#include <network/protocol/logic/sources/ConnectionSource.h>
#include <network/protocol/context/ProcessorContext.h>


ConnectionProcessor::ConnectionProcessor(Connection &connection) : connection(connection) {

}

void ConnectionProcessor::run() {
    logger.info("ConnectionProcessor start");
    //set up context
    Context &context = connection.getConnectionContext();
    Context::setActiveContext(&context);

    auto lc = context.get<LogicContext>();
    auto &logicManager = lc->getLogicManager();
    auto connectionSourcePtr = logicManager.getSource<ConnectionSource>();

    auto processorContext = context.set<ProcessorContext, ConnectionProcessor &>(*this);

    while (!this->isStopping()) {
        auto np = connection.receive();
        if (np != nullptr) {
            processorContext->setThisPacket(np);
            np->process(context);
        } else {
            ///@todo error handling
        }

    }

}

ConnectionProcessor::~ConnectionProcessor() {

    this->stop();

}

