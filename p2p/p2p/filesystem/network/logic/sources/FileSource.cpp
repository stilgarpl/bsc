//
// Created by stilgar on 11.10.17.
//

#include "FileSource.h"
#include <p2p/network/protocol/context/ConnectionContext.h>
#include <p2p/filesystem/network/packet/SendFile.h>

void FileSource::requestFile(SendFile::Request *request) {
    // auto connectionContext = Context::getActiveContext().get<ConnectionContext>();

    auto event = requestSource.newEvent();
    if (request->getEnd() == 0) {
        event->setEventId(FileRequestId::GET_FILE);
    } else {
        event->setEventId(FileRequestId::GET_CHUNK);
    }
    //whole file
    event->setBegin(request->getBegin());
    event->setEnd(request->getEnd());
    event->setFilePath(request->getFilePath());
    // event->setConnection(&connectionContext->getConnection());
    event->setRequestId(request->getId());

//    event->setNetworkInfo(networkInfo);
//    event->setEventId(EventType::IdType::NETWORK_INFO_RECEIVED);

    requestSource.queueEvent(event);

}

void FileSource::work() {
    requestSource.work();
    responseSource.work();

}

void FileSource::registerProviders(SourceManager *manager) {
    requestSource.registerProviders(manager);
    responseSource.registerProviders(manager);
}

void FileSource::fileReceived(SendFile::Response::Ptr response) {
    //auto connectionContext = Context::getActiveContext().get<ConnectionContext>();

    auto event = responseSource.newEvent();
    if (response->getEnd() == 0) {
        event->setEventId(FileResponseId::FILE_RECEIVED);
    } else {
        event->setEventId(FileResponseId::CHUNK_RECEIVED);
    }
    //whole file
    //  event->setResponse(response);
    //  SendFile::Response* response;
    event->setFilePath(response->getFilePath());
    ///@todo remove debug override
    event->setFilePath("/tmp/basyco/received.file");
    event->setBegin(response->getBegin());
    event->setEnd(response->getEnd());
    event->setData(response->getData());

    responseSource.queueEvent(event);
}
