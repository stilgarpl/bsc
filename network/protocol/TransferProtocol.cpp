//
// Created by stilgar on 31.07.17.
//

#include "TransferProtocol.h"
#include <Poco/ByteOrder.h>

void TransferProtocol::send(Poco::Int16 length, const void *data) {

    int bytesSent = socket.sendBytes(data, length);
    if (bytesSent < length) {
        //@TODO error
    }
}

void TransferProtocol::receive() {

    Poco::Int16 length;
    socket.receiveBytes(&length, sizeof length);
    Poco::Int8 data[length];
    socket.receiveBytes(data, length);

}


void TransferProtocol::send(Poco::Int16 control, Poco::Int8 packetType, Poco::Int16 dataLength, const void *data) {

//    socket.sendBytes(reinterpret_cast<const void *>(Poco::ByteOrder::toNetwork(dataLength+3)),sizeof Poco::Int16);
//    socket.sendBytes(reinterpret_cast<const void *>(Poco::ByteOrder::toNetwork(control)), sizeof control);
//    socket.sendBytes(reinterpret_cast<const void *>(Poco::ByteOrder::toNetwork(packetType)), sizeof packetType);
//    //@todo don't like the way it's programmed, it should send everything through this or I don't know, whatever is more efficient
//    send(dataLength,data);

}
