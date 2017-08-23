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
#include "../../../context/Context.h"
#include "../enum/Status.h"


class NetworkPacket {
private:
    Status status;
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & status;
    }

    friend class cereal::access;

public:

    virtual void process(Context &context);

    Status getStatus() const;

    void setStatus(Status status);
};


CEREAL_REGISTER_TYPE(NetworkPacket);

#endif //BASYCO_NETWORKPACKET_H
