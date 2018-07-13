//
// Created by stilgar on 11.10.17.
//

#ifndef BASYCO_FILEACTIONS_H
#define BASYCO_FILEACTIONS_H


#include <p2p/modules/filesystem/network/logic/events/FileRequestEvent.h>
#include <p2p/modules/filesystem/network/packet/SendFile.h>
#include <p2p/modules/filesystem/network/logic/events/FileResponseEvent.h>

class FileActions {

public:

    static void sendFile(const FileRequestEvent &event);

    static void receivedFile(const FileResponseEvent &event);

    static void sendAttributes(const FileAttributesEvent &event);

    static void receivedAttributes(const FileAttributesEvent &event);

};


#endif //BASYCO_FILEACTIONS_H
