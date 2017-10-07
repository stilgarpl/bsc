//
// Created by stilgar on 01.08.17.
//

#ifndef BASYCO_NETWORKPACKET_H
#define BASYCO_NETWORKPACKET_H
//#define CEREAL_THREAD_SAFE 1


#include "../enum/Command.h"
#include "../../../context/Context.h"
#include "../enum/Status.h"

#include "../../../utils/cereal_include.h"
#include "../../../log/Logger.h"

class BasePacket {
public:
    typedef unsigned int IdType;
private:
    IdType nextId() {
        static IdType val = 0;
        return val++;
    };
    Status status;
    IdType id;
    bool retry = false;
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(status, id, retry);
    }

    friend class cereal::access;

public:

    ///@todo pure virtual perhaps? or maybe that would mess up ACK? I can create AckPacket...
    virtual void process(Context &context);

    Status getStatus() const;

    void setStatus(Status status);

    IdType getId() const;

    void setId(IdType id);

    BasePacket(const BasePacket &) = delete;

    BasePacket() : id(nextId()) {};

    bool isRetry() const;

    void setRetry(bool retry);

};


typedef std::shared_ptr<BasePacket> BasePacketPtr;

template<typename NetworkPacketType = BasePacket>
using NetworkPacketPointer = std::shared_ptr<NetworkPacketType>;


CEREAL_REGISTER_TYPE(BasePacket);

#endif //BASYCO_NETWORKPACKET_H
