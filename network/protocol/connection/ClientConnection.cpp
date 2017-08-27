//
// Created by stilgar on 01.08.17.
//

#include <Poco/Net/SocketStream.h>
#include "ClientConnection.h"


ClientConnection::ClientConnection(const Poco::Net::SocketAddress &a, Context &context) : socket(a),
                                                                                          Connection(context) {
    socket.setReceiveTimeout(Poco::Timespan(20, 0));

}
