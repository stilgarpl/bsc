//
// Created by stilgar on 11.10.17.
//

#ifndef BASYCO_FILEREQUESTEVENT_H
#define BASYCO_FILEREQUESTEVENT_H

#include <experimental/filesystem>
#include <p2p/logic/IEvent.h>
#include <p2p/network/protocol/connection/Connection.h>

namespace fs = std::experimental::filesystem;

enum class FileRequestId {
    GET_FILE,
    GET_CHUNK,
    GET_ATTRIBUTES, //? this event or other?
};

class FileRequestEvent : public IEvent<FileRequestId> {

private:
    fs::path filePath;
    size_t begin, end;
    //@todo think about it: Is it necessary? Can we respond in some other way?
    Connection *connection;
    BasePacket::IdType requestId;

public:
    const std::experimental::filesystem::path &getFilePath() const;

    void setFilePath(const std::experimental::filesystem::path &filePath);

    size_t getBegin() const;

    void setBegin(size_t begin);

    size_t getEnd() const;

    void setEnd(size_t end);

    Connection *getConnection() const;

    void setConnection(Connection *connection);

    BasePacket::IdType getRequestId() const;

    void setRequestId(BasePacket::IdType requestId);


};


#endif //BASYCO_FILEREQUESTEVENT_H