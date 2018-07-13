//
// Created by stilgar on 11.10.17.
//

#include <p2p/modules/filesystem/network/logic/events/FileAttributesEvent.h>
#include <p2p/modules/filesystem/network/packet/SendAttributes.h>
#include "FileActions.h"

void FileActions::sendFile(const FileRequestEvent &event) {
    Connection *connection = event.origin();


    if (fs::exists(event.getFilePath())) {
        SendFile::Response::Ptr response = SendFile::Response::getNew<Status::RESPONSE>(event.getRequestId());
        //begin and end must be set before the loading of the file
        response->setBegin(event.getBegin());
        response->setEnd(event.getEnd());
        response->load_file(event.getFilePath());
        connection->send(response);
    } else {
        auto response = SendFile::Response::getNew<Status::ERROR>(event.getRequestId());
        connection->send(response);
    }

}

void FileActions::receivedFile(const FileResponseEvent &event) {
    //event.setFilePath("/tmp/basyco/received.file");
    // event.saveFile();
}

void FileActions::sendAttributes(const FileAttributesEvent &event) {
    Connection *connection = event.origin();
    LOGGER("file actions attr packet id" + std::to_string(event.getRequestId()));

    if (fs::exists(event.getPath())) {
        SendAttributes::Response::Ptr response = SendAttributes::Response::getNew<Status::RESPONSE>(
                event.getRequestId());
        //SendAttributes::Response* response;
        LOGGER("getting size r" + std::to_string(response->getId()))
        //get the attributes
        response->setSize(fs::file_size(event.getPath()));
        LOGGER("size is " + std::to_string(response->getSize()))
        connection->send(response);
    } else {
        auto response = SendAttributes::Response::getNew<Status::ERROR>(event.getRequestId());
        connection->send(response);
    }
}

void FileActions::receivedAttributes(const FileAttributesEvent &event) {

}
