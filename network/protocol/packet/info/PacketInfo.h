//
// Created by stilgar on 08.10.17.
//

#ifndef BASYCO_PACKETINFO_H
#define BASYCO_PACKETINFO_H

#include <network/protocol/enum/Status.h>
#include <network/protocol/packet/BasePacket.h>

template<typename PacketType, enum Status s>
struct PacketInfo {

};

//typedef typename PacketInfo<typename PacketType::BaseType, s>::Type Type;


//template partial specialization, if new Status is added, it should be specialized here

template<typename PacketType>
struct PacketInfo<PacketType, Status::REQUEST> {
    typedef typename PacketType::Request Type;
};

template<typename PacketType>
struct PacketInfo<PacketType, Status::RESPONSE> {
    typedef typename PacketType::Response Type;
};

template<typename PacketType>
struct PacketInfo<PacketType, Status::ACK> {
    typedef typename PacketType::Ack Type;
};

template<typename PacketType>
struct PacketInfo<PacketType, Status::ERROR> {
    typedef typename PacketType::Error Type;
};


struct PacketUtils {

//    template<enum Status s, typename T>
//    static typename PacketInfo<typename T::BaseType, s>::Type getNew(T t) {
//        typename PacketInfo<typename T::BaseType, s>::Type ret;
//        ret.setStatus(s);
//        ret.setId(t.getId());
//        return ret;
//    };
//
//    template<enum Status s, typename T>
//    static typename PacketInfo<typename T::BaseType, s>::Type getNew(T t, BasePacket::IdType id) {
//        auto ret = getNew<s>(t);
//        ret.setId(id);
//        return ret;
//    };

    template<enum Status s, typename T>
    static auto getNewPtr(T *t) {
//        std::shared_ptr<typename PacketInfo<typename T::BaseType, s>::Type> ret = std::make_shared<typename PacketInfo<typename T::BaseType, s>::Type>();
//        ret->setStatus(s);
//        ret->setId(t->getId());
//        return ret;

        auto ptr = getNewPtr<s, T>();
        ptr->setId(t->getId());
        return ptr;
    };

    template<enum Status s, typename T>
    static auto getNewPtr() {
        std::shared_ptr<typename PacketInfo<typename T::BaseType, s>::Type> ret = std::make_shared<typename PacketInfo<typename T::BaseType, s>::Type>();
        ret->setStatus(s);
        // ret->setId(t->getId());
        return ret;
    };

//    template<enum Status s, typename T>
//    static auto getNewPtr(T* t, BasePacket::IdType id) {
//        auto ret = getNewPtr<s>(t);
//        ret->setId(id);
//        return ret;
//    };

};

//template< typename T, enum Status status>
//struct Packet : public BasePacket {
//    typedef T BaseType;
//
//    static auto getNew() {
//        return PacketUtils::getNewPtr<status,T>();
//    }
//
//    constexpr static Status getPacketStatus() {
//        return status;
//
//    };
//
//    Packet() {
//        setStatus(getPacketStatus());
//    }
//
//};


/**
 * Base class for all packets that use packet grouping feature.
 * @tparam T Packet Group class, must inherit from PacketGroup class
 * @tparam S packet class inheriting from Packet
 * @tparam overrideStatus DO NOT USE, INTERNAL USE ONLY
 * @tparam sts DO NOT USE, INTERNAL USE ONLY
 */
template<typename T, typename S = BasePacket, bool overrideStatus = false, enum Status sts = Status::ERROR>
struct Packet : public BasePacket {
    typedef T BaseType;


    template<enum Status ss, typename Type>
    static auto getNew(Type *t) {
        return PacketUtils::getNewPtr<ss>(t);
    }

    template<enum Status ss>
    static auto getNew() {
        return PacketUtils::getNewPtr<ss, typename PacketInfo<T, ss>::Type>();
    }

    static auto getNew() {
        //return std::make_shared<S>();
        return getNew<getPacketStatus()>();
    }

    constexpr static Status getPacketStatus() {
        if (overrideStatus) return sts;
        if (std::is_same<typename T::Request, S>::value) return Status::REQUEST;
        if (std::is_same<typename T::Response, S>::value) return Status::RESPONSE;
        if (std::is_same<typename T::Ack, S>::value) return Status::ACK;
        if (std::is_same<typename T::Error, S>::value) return Status::ERROR;
        return Status::ERROR;
    };

    Packet() {
        setStatus(getPacketStatus());
    }
//
//
//    static auto getNew() {
//        return PacketUtils::getNewPtr<s,typename PacketInfo<T,s>::Type>();
//    }


};


struct PacketGroup {

    typedef Packet<PacketGroup, Packet<PacketGroup>, true, Status::REQUEST> Request;
    typedef Packet<PacketGroup, Packet<PacketGroup>, true, Status::RESPONSE> Response;
    typedef Packet<PacketGroup, Packet<PacketGroup>, true, Status::ERROR> Error;
    typedef Packet<PacketGroup, Packet<PacketGroup>, true, Status::ACK> Ack;

};


#endif //BASYCO_PACKETINFO_H
