//
// Created by stilgar on 11.10.17.
//

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
    event.saveFile();
}
