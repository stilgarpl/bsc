//
// Created by stilgar on 11.10.17.
//

#ifndef BASYCO_FILEACTIONS_H
#define BASYCO_FILEACTIONS_H


#include <filesystem/network/logic/events/FileRequestEvent.h>
#include <filesystem/network/packet/SendFile.h>
#include <filesystem/network/logic/events/FileResponseEvent.h>

class FileActions {

public:

    static void sendFile(const FileRequestEvent &event) {
        Connection *connection = event.getConnection();


        if (fs::exists(event.getFilePath())) {
            SendFile::Response::Ptr response = SendFile::Response::getNew<Status::RESPONSE>(event.getRequestId());
            response->load_file(event.getFilePath());
            connection->send(response);
        } else {
            auto response = SendFile::Response::getNew<Status::ERROR>(event.getRequestId());
            connection->send(response);
        }

    }

    static void receivedFile(const FileResponseEvent &event) {
        event.getResponse()->setFilePath("/tmp/basyco/received.file");
        event.getResponse()->save_file();
    }

};


#endif //BASYCO_FILEACTIONS_H
