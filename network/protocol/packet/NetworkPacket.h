//
// Created by stilgar on 01.08.17.
//

#ifndef BASYCO_NETWORKPACKET_H
#define BASYCO_NETWORKPACKET_H
//#define CEREAL_THREAD_SAFE 1

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../enum/Command.h"


class NetworkPacket {
private:
    Command command;

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & command;
    }

    friend class cereal::access;

public:
    Command getCommand() const {
        return command;
    }

    void setCommand(Command command) {
        NetworkPacket::command = command;
    }



};


//CEREAL_REGISTER_TYPE(NetworkPacket);

#endif //BASYCO_NETWORKPACKET_H
