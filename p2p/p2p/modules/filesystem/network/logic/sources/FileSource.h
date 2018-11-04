//
// Created by stilgar on 11.10.17.
//

#ifndef BASYCO_FILESOURCE_H
#define BASYCO_FILESOURCE_H


#include <p2p/logic/ISource.h>
#include <filesystem>
#include <p2p/modules/filesystem/network/packet/SendFile.h>
#include <p2p/logic/sources/EventQueueSource.h>
#include <p2p/modules/filesystem/network/logic/events/FileRequestEvent.h>
#include <p2p/modules/filesystem/network/logic/events/FileResponseEvent.h>
#include <p2p/modules/filesystem/network/packet/SendAttributes.h>
#include <p2p/modules/filesystem/network/logic/events/FileAttributesEvent.h>


namespace fs = std::filesystem;


class FileSource : public ISource {
private:
    EventQueueSource<FileRequestEvent, FileSource> requestSource;
    EventQueueSource<FileResponseEvent, FileSource> responseSource;
    EventQueueSource<FileAttributesEvent, FileSource> attributesSource;
public:
    FileSource(SourceManager &sourceManager);

//@todo * -> ::Ptr
    void requestFile(SendFile::Request *request);

    void fileReceived(SendFile::Response::Ptr response);

    void requestAttributes(SendAttributes::Request *request) {
        auto event = attributesSource.newEvent(request->getId());

        event->setPath(request->getPath());
//        event->setRequestId(request->getId());
        attributesSource.queueEvent(event);
    };

    void attributesReceived() {};



public:
    void work() override;

};


#endif //BASYCO_FILESOURCE_H