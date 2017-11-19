//
// Created by stilgar on 16.11.17.
//

#ifndef BASYCO_COMMANDPACKET_H
#define BASYCO_COMMANDPACKET_H


#include <p2p/network/protocol/packet/info/PacketInfo.h>

class CommandPacket : PacketGroup {


    class Request : public Packet<CommandPacket, CommandPacket::Request> {
    private:
        std::vector<std::string> modules;
        std::string commandName;
        std::vector<std::string> data;
    public:
        void process(Context &context) override;
    };


    class Response : public Packet<CommandPacket, CommandPacket::Response> {
    public:
        void process(Context &context) override;
    };

};


#endif //BASYCO_COMMANDPACKET_H
