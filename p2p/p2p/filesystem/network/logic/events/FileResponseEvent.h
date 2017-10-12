//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_FILERESPONSEEVENT_H
#define BASYCO_FILERESPONSEEVENT_H

#include <p2p/logic/IEvent.h>
#include <p2p/filesystem/network/packet/SendFile.h>

enum class FileResponseId {
    FILE_RECEIVED,
    CHUNK_RECEIVED,
};

class FileResponseEvent : public IEvent<FileResponseId> {
private:
    SendFile::Response::Ptr response;
public:
    auto getResponse() const {
        return response;
    }

    void setResponse(const SendFile::Response::Ptr r) {
        response = r;
    }

};


#endif //BASYCO_FILERESPONSEEVENT_H
