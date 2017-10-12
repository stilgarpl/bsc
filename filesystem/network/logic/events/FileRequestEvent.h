//
// Created by stilgar on 11.10.17.
//

#ifndef BASYCO_FILEREQUESTEVENT_H
#define BASYCO_FILEREQUESTEVENT_H

#include <experimental/filesystem>
#include <logic/IEvent.h>
#include <network/protocol/connection/Connection.h>

namespace fs = std::experimental::filesystem;

enum class FileRequestId {
    GET_FILE,
    GET_CHUNK,
    GET_ATTRIBUTES, //? this event or other?
};

class FileRequestEvent : public IEvent<FileRequestId> {

private:
    fs::path filePath;
    int begin, end;
    //@todo think about it: Is it necessary? Can we respond in some other way?
    Connection *connection;
    BasePacket::IdType requestId;

public:
    const std::experimental::filesystem::path &getFilePath() const;

    void setFilePath(const std::experimental::filesystem::path &filePath);

    int getBegin() const;

    void setBegin(int begin);

    int getEnd() const;

    void setEnd(int end);

    Connection *getConnection() const;

    void setConnection(Connection *connection);

    BasePacket::IdType getRequestId() const;

    void setRequestId(BasePacket::IdType requestId);


};


#endif //BASYCO_FILEREQUESTEVENT_H
