//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_FILERESPONSEEVENT_H
#define BASYCO_FILERESPONSEEVENT_H

#include <logic/IEvent.h>
#include <filesystem/network/packet/SendFile.h>

enum class FileResponseId {
    FILE_RECEIVED,
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
