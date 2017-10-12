//
// Created by stilgar on 11.10.17.
//

#include "FileSource.h"
#include <network/protocol/context/ConnectionContext.h>
#include <filesystem/network/packet/SendFile.h>

void FileSource::requestFile(SendFile::Request *request) {
    auto connectionContext = Context::getActiveContext().get<ConnectionContext>();

    auto event = requestSource.newEvent();
    event->setEventId(FileRequestId::GET_FILE);
    //whole file
    event->setBegin(0);
    event->setEnd(0);
    event->setFilePath(request->getFilePath());
    event->setConnection(&connectionContext->getConnection());
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
    auto connectionContext = Context::getActiveContext().get<ConnectionContext>();

    auto event = responseSource.newEvent();
    event->setEventId(FileResponseId::FILE_RECEIVED);
    //whole file
    event->setResponse(response);

    responseSource.queueEvent(event);
}
