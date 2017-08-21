//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_COMMANDPACKET_H
#define BASYCO_COMMANDPACKET_H


#include "NetworkPacket.h"
#include "../../../log/Logger.h"

class CommandPacket : public NetworkPacket {
private:
    Logger logger = Logger("CommandPacket");


private:
    Command command;
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & command;
    }

    friend class cereal::access;

public:
    void process(Context &context) override;

    Command getCommand() const;

    void setCommand(Command command);
};

CEREAL_REGISTER_TYPE(CommandPacket);

CEREAL_REGISTER_POLYMORPHIC_RELATION(NetworkPacket, CommandPacket);

#endif //BASYCO_COMMANDPACKET_H
