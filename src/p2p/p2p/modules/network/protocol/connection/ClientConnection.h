//
// Created by Krzysztof Tulidowicz on 01.08.17.
//

#ifndef BSC_CLIENTCONNECTION_H
#define BSC_CLIENTCONNECTION_H


#include <Poco/Net/StreamSocket.h>
#include "Connection.h"


namespace bsc {
    class ClientConnection : public Connection {

    private:
        Poco::Net::StreamSocket socket;

    protected:
        ClientConnection(const Poco::Net::SocketAddress& a, const Context::Ptr&);

    public:
        ClientConnection(const std::string& a, const Context::Ptr&);

        //@todo think about the Connection interface and those method names.
        void startReceivingImpl();

        void startSendingImpl();

        ~ClientConnection() override;

        void shutdown() override;

    protected:
        Poco::Net::StreamSocket& getSocket() override;

    };
}


#endif //BSC_CLIENTCONNECTION_H
