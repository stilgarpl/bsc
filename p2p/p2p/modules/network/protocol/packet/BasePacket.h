//
// Created by stilgar on 01.08.17.
//

#ifndef BASYCO_BASEPACKET_H
#define BASYCO_BASEPACKET_H
//#define CEREAL_THREAD_SAFE 1



#include <p2p/modules/network/protocol/enum/Status.h>
#include <core/utils/cereal_include.h>
#include <core/log/Logger.h>

#include <core/context/Context.h>
#include <p2p/role/Roles.h>

struct PacketGroup;

class PacketSourceWorker;

class BasePacket;

template<typename NetworkPacketType = BasePacket>
using NetworkPacketPointer = std::shared_ptr<NetworkPacketType>;

class BasePacket {
public:
    typedef unsigned int IdType;
    typedef std::shared_ptr<BasePacket> Ptr;
    typedef PacketGroup BaseType;
    std::mutex idLock;
private:
    //@fixme Ids are not unique across nodes! Is that a problem for Transmission Control or Graviton? Probably not, but investigate
    /// what if A sends id 5 to B and then C sends id 5 to B ? would that work?
    IdType nextId();
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

    //@todo pure virtual perhaps? or maybe that would mess up ACK? I can create AckPacket...
    virtual void process(Context::Ptr context);

    Status getStatus() const;

    void setStatus(Status status);

    IdType getId() const;

    void setId(IdType id);

    //I don't remember why it was deleted, probably because of serialization bug(fixed) or maybe to not waste ids (who cares about ids?)
    // BasePacket(const BasePacket &) = delete;

    BasePacket() : status(Status::REQUEST), id(nextId()) {};

    bool isRetry() const;

    void setRetry(bool retry);

    virtual const RoleList &requiredRoles() =0;

    void resetId() {
        setId(nextId());
    }


protected:

    virtual void _operate(PacketSourceWorker &f, std::shared_ptr<BasePacket>) = 0;

    friend class ConnectionSource;

    virtual ~BasePacket() = default;
};

typedef std::shared_ptr<BasePacket> BasePacketPtr;


template<typename... T1>
using Fu = std::function<void(T1 &...)>;

class TypeOperator {

    template<typename T>
    void operate(NetworkPacketPointer<T> ptr) {
        LOGGER("do shit")
        //@todo somehow invoke logic engine. preferably without including logic manager here.
        //  if only somehow I could pass a lambda or sth that would be invoked here.

    }
};



CEREAL_REGISTER_TYPE(BasePacket)

#endif //BASYCO_BASEPACKET_H