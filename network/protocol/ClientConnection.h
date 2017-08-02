//
// Created by stilgar on 01.08.17.
//

#ifndef BASYCO_CLIENTCONNECTION_H
#define BASYCO_CLIENTCONNECTION_H


#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include "NetworkPacket.h"

class ClientConnection {

private:
    std::shared_ptr<Poco::Net::StreamSocket> socket;

public:
    ClientConnection(std::shared_ptr<Poco::Net::StreamSocket> socket);

    void send(std::shared_ptr<NetworkPacket> np);

    template<typename T>
    void send(std::shared_ptr<T> np) {
        Poco::Net::SocketStream stream(*socket);
        cereal::BinaryOutputArchive oa(stream);
        oa << np;
    };

    template<typename T>
    std::shared_ptr<T> receive() {
        std::shared_ptr<T> np;
        Poco::Net::SocketStream stream(*socket);
        cereal::BinaryInputArchive ia(stream);
        ia >> np;
    }

};


#endif //BASYCO_CLIENTCONNECTION_H
