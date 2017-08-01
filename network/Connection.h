//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_CONNECTION_H
#define BASYCO_CONNECTION_H

#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>

class Connection : public Poco::Net::TCPServerConnection {

public:
    void run() override;

    Connection(const Poco::Net::StreamSocket &socket);
};


#endif //BASYCO_CONNECTION_H
