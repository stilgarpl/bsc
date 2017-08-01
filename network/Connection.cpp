//
// Created by stilgar on 31.07.17.
//

#include <thread>
#include <iostream>
#include "Connection.h"

using namespace std::chrono_literals;

void Connection::run() {

    auto streamSocket = socket();

    while (true) {
        if (streamSocket.available() > 0) {
            int bytes = 10;
            streamSocket.available();
            if (bytes < 0) {
                break;
            }
            char buffer[bytes + 1];
            int n = streamSocket.receiveBytes(buffer, bytes);
            buffer[bytes] = '\0';
            std::cout << "debug received " << buffer << std::endl;
        } else {
            std::this_thread::sleep_for(1s);
        }


    }

    std::cout << "CLOSING CONNECTION";

}

Connection::Connection(const Poco::Net::StreamSocket &socket) : TCPServerConnection(socket) {}
