//
// Created by stilgar on 11.10.17.
//

#ifndef BASYCO_FILESOURCE_H
#define BASYCO_FILESOURCE_H


#include <p2p/logic/ISource.h>
#include <experimental/filesystem>
#include <p2p/filesystem/network/packet/SendFile.h>
#include <p2p/logic/sources/EventQueueSource.h>
#include <p2p/filesystem/network/logic/events/FileRequestEvent.h>
#include <p2p/filesystem/network/logic/events/FileResponseEvent.h>


namespace fs = std::experimental::filesystem;


class FileSource : public ISource {
private:
    EventQueueSource<FileRequestEvent, FileSource> requestSource;
    EventQueueSource<FileResponseEvent, FileSource> responseSource;
public:
    //@todo * -> ::Ptr
    void requestFile(SendFile::Request *request);

    void fileReceived(SendFile::Response::Ptr response);

public:
    void work() override;

    void registerProviders(SourceManager *manager) override;

};


#endif //BASYCO_FILESOURCE_H