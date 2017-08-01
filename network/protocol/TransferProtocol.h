//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_PROTOCOL_H
#define BASYCO_PROTOCOL_H


#include <Poco/Types.h>
#include <Poco/Net/StreamSocket.h>
#include <memory>

class TransferProtocol {

private:
    Poco::Net::StreamSocket &socket;

protected:
    void send(Poco::Int16 length, const void *data);

public:
    void send(Poco::Int16 control, Poco::Int8 packetType, Poco::Int16 dataLength, const void *data);

    void receive();

public:
    TransferProtocol(std::shared_ptr<Poco::Net::StreamSocket> &socket);

};


#endif //BASYCO_PROTOCOL_H
