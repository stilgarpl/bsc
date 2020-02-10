//
// Created by stilgar on 31.07.17.
//

#ifndef BSC_SERVER_CONNECTION_H
#define BSC_SERVER_CONNECTION_H

#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <p2p/core/node/Node.h>
#include "Connection.h"
#include "IServerConnection.h"


namespace bsc {
    class ServerConnection : public Poco::Net::TCPServerConnection, public bsc::IServerConnection {

    public:
        void run() override;

        ServerConnection(const Poco::Net::StreamSocket& socket, Context::Ptr context);

        void startReceiving(Poco::Net::StreamSocket& socket) override;

        void stopReceiving() override;

        virtual ~ServerConnection();

        void stop() override;

        void shutdown() override;

    protected:
        Poco::Net::StreamSocket& getSocket() override;
    };

    class ServerConnectionFactory : public Poco::Net::TCPServerConnectionFactory {
    private:
        std::function<Context::OwnPtr(void)> contextGetter;
        std::list<std::reference_wrapper<Connection::ObserverType>> observers;
    public:

        ServerConnectionFactory(std::function<Context::OwnPtr(void)> contextGetter,
                                std::list<std::reference_wrapper<Connection::ObserverType>> observers);

        Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket) override;

    };
}


#endif //BSC_SERVER_CONNECTION_H
