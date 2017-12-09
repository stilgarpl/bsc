//
// Created by stilgar on 16.11.17.
//

#ifndef BASYCO_COMMANDPACKET_H
#define BASYCO_COMMANDPACKET_H


#include <p2p/network/protocol/packet/info/PacketInfo.h>
#include <p2p/utils/cereal_include.h>

class CommandPacket : public PacketGroup {
public:

    class Request : public Packet<CommandPacket, CommandPacket::Request> {
    private:
        std::vector<std::string> modules;
        std::string commandName;
        std::vector<std::string> data;
    public:
        void process(Context &context) override;

        const std::vector<std::string> &getModules() const;

        void setModules(const std::vector<std::string> &modules);

        const std::string &getCommandName() const;

        void setCommandName(const std::string &commandName);

        const std::vector<std::string> &getData() const;

        void setData(const std::vector<std::string> &data);

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar & cereal::base_class<Packet<CommandPacket, CommandPacket::Request>>(this);

            ar(modules, commandName, data);
        }


        friend class cereal::access;
    };


    class Response : public Packet<CommandPacket, CommandPacket::Response> {
        bool runStatus;
    public:
        void process(Context &context) override;

        bool isRunStatus() const;

        void setRunStatus(bool runStatus);

    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar & cereal::base_class<Packet<CommandPacket, CommandPacket::Response>>(this);
            ar & runStatus;
        }


        friend class cereal::access;
    };

};

CEREAL_REGISTER_TYPE(CommandPacket::Request);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, CommandPacket::Request);
CEREAL_REGISTER_TYPE(CommandPacket::Response);
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, CommandPacket::Response);

#endif //BASYCO_COMMANDPACKET_H
