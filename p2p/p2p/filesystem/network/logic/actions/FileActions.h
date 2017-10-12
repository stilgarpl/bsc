//
// Created by stilgar on 11.10.17.
//

#ifndef BASYCO_FILEACTIONS_H
#define BASYCO_FILEACTIONS_H


#include <p2p/filesystem/network/logic/events/FileRequestEvent.h>
#include <p2p/filesystem/network/packet/SendFile.h>
#include <p2p/filesystem/network/logic/events/FileResponseEvent.h>

class FileActions {

public:

    static void sendFile(const FileRequestEvent &event);

    static void receivedFile(const FileResponseEvent &event);

};


#endif //BASYCO_FILEACTIONS_H
