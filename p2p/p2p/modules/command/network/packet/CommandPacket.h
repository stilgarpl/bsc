//
// Created by stilgar on 16.11.17.
//

#ifndef BSC_COMMANDPACKET_H
#define BSC_COMMANDPACKET_H


#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <core/utils/cereal_include.h>


namespace bsc {
    class CommandPacket : public PacketGroup {
    public:

        class Request : public bsc::Packet<CommandPacket, CommandPacket::Request> {
        private:
            std::string commandName;
            std::vector<std::string> data;
        public:


            const std::string& getCommandName() const;

            void setCommandName(const std::string& commandName);

            const std::vector<std::string>& getData() const;

            void setData(const std::vector<std::string>& data);

        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar & cereal::base_class<Packet<CommandPacket, CommandPacket::Request>>(this);

                ar(commandName, data);
            }


            friend class cereal::access;
        };


        class Response : public bsc::Packet<CommandPacket, CommandPacket::Response> {
            bool runStatus;
            std::string output;
        public:

            bool isRunStatus() const;

            void setRunStatus(bool runStatus);

            const std::string& getOutput() const;

            void setOutput(const std::string& output);

        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar & cereal::base_class<Packet<CommandPacket, CommandPacket::Response>>(this);
                ar & runStatus & output;
            }


            friend class cereal::access;
        };

    };
}

CEREAL_REGISTER_TYPE(bsc::CommandPacket::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::CommandPacket::Request)

CEREAL_REGISTER_TYPE(bsc::CommandPacket::Response)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::CommandPacket::Response)

#endif //BSC_COMMANDPACKET_H
