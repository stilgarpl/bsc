//
// Created by stilgar on 01.08.17.
//

#ifndef BASYCO_CLIENTCONNECTION_H
#define BASYCO_CLIENTCONNECTION_H


#include <Poco/Net/StreamSocket.h>
#include "protocol/NetworkPacket.h"

class ClientConnection {

private:
    std::shared_ptr<Poco::Net::StreamSocket> socket;

public:
    ClientConnection(std::shared_ptr<Poco::Net::StreamSocket> socket);

    void send(std::shared_ptr<NetworkPacket> np);

    std::shared_ptr<NetworkPacket> receive();

};


#endif //BASYCO_CLIENTCONNECTION_H
