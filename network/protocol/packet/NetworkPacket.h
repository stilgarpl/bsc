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
public:
    typedef unsigned int IdType;
private:
    IdType nextId() {
        static IdType val = 0;
        return val++;
    };
    Status status;
    IdType id = nextId();
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & status;
        ar & id;
    }

    friend class cereal::access;

public:

    virtual void process(Context &context);

    Status getStatus() const;

    void setStatus(Status status);

    IdType getId() const;

    void setId(IdType id);

};


typedef std::shared_ptr<NetworkPacket> NetworkPacketPtr;

CEREAL_REGISTER_TYPE(NetworkPacket);

#endif //BASYCO_NETWORKPACKET_H
