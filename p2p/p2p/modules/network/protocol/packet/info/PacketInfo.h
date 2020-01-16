//
// Created by stilgar on 08.10.17.
//

#ifndef BASYCO_PACKETINFO_H
#define BASYCO_PACKETINFO_H

#include <p2p/modules/network/protocol/enum/Status.h>
#include <p2p/modules/network/protocol/packet/BasePacket.h>
#include <p2p/modules/network/protocol/logic/sources/ConnectionSource.h>


namespace bsc {
    template<typename PacketType, enum Status s>
    struct PacketInfo {

    };
    template<typename PacketType>
    struct PacketInfo<PacketType, Status::request> {
        typedef typename PacketType::Request Type;
    };
    template<typename PacketType>
    struct PacketInfo<PacketType, Status::response> {
        typedef typename PacketType::Response Type;
    };
    template<typename PacketType>
    struct PacketInfo<PacketType, Status::ack> {
        typedef typename PacketType::Ack Type;
    };
    template<typename PacketType>
    struct PacketInfo<PacketType, Status::error> {
        typedef typename PacketType::Error Type;
    };

    struct PacketUtils {

        template<enum Status s, typename PacketType>
        static auto getNewPtr(PacketType* t) {
            //        std::shared_ptr<typename PacketInfo<typename GroupType::BaseType, s>::Type> ret = std::make_shared<typename PacketInfo<typename GroupType::BaseType, s>::Type>();
            //        ret->setStatus(s);
            //        ret->setId(t->getId());
            //        return ret;

            auto ptr = getNewPtr<s, PacketType>();
            ptr->setId(t->getId());
            return ptr;
        }

        //used for shared_ptr<Packet>
        template<enum Status s, typename PacketTypePtr>
        static auto getNewPtr(const PacketTypePtr& t) {

            return getNewPtr<s>(t.get());
        }

        template<enum Status s, typename PacketType>
        static auto getNewPtr() {
            std::shared_ptr<typename PacketInfo<typename PacketType::BaseType, s>::Type> ret = std::make_shared<typename PacketInfo<typename PacketType::BaseType, s>::Type>();
            ret->setStatus(s);
            // ret->setId(t->getId());
            return ret;
        }

        template<enum Status s, typename T>
        static auto getNewPtr(const BasePacket::IdType& id) {
            std::shared_ptr<typename PacketInfo<typename T::BaseType, s>::Type> ret = std::make_shared<typename PacketInfo<typename T::BaseType, s>::Type>();
            ret->setStatus(s);
            ret->setId(id);
            return ret;
        }
    };


/**
 * Base class for all packets that use packet grouping feature.
 * @tparam GroupType Packet Group class, must inherit from PacketGroup class
 * @tparam S packet class inheriting from Packet
 * @tparam overrideStatus DO NOT USE, INTERNAL USE ONLY
 * @tparam sts DO NOT USE, INTERNAL USE ONLY
 */
    template<typename GroupType, typename S = BasePacket, bool overrideStatus = false, enum Status sts = Status::error>
    struct Packet : public BasePacket {
        typedef GroupType BaseType;
        typedef NetworkPacketPointer<S> Ptr;


        template<enum Status ss, typename Type>
        static auto getNew(Type* t) {
            return PacketUtils::getNewPtr<ss>(t);
        }

        template<enum Status ss>
        static auto getNew() {
            return PacketUtils::getNewPtr<ss, typename PacketInfo<GroupType, ss>::Type>();
        }

        static auto getNew() {
            //return std::make_shared<S>();
            return getNew<getPacketStatus()>();
        }

        template<typename Type>
        static auto getNew(Type* t) {
            return getNew<getPacketStatus()>(t);
        }

        template<enum Status ss>
        static auto getNew(const BasePacket::IdType& id) {
            return PacketUtils::getNewPtr<ss, typename PacketInfo<GroupType, ss>::Type>(id);
        }

        static auto getNew(const BasePacket::IdType& id) {
            //return std::make_shared<S>();
            return getNew<getPacketStatus()>(id);
        }

        constexpr static Status getPacketStatus() {
            if (overrideStatus) return sts;
            if (std::is_same<typename GroupType::Request, S>::value) return Status::request;
            if (std::is_same<typename GroupType::Response, S>::value) return Status::response;
            if (std::is_same<typename GroupType::Ack, S>::value) return Status::ack;
            if (std::is_same<typename GroupType::Error, S>::value) return Status::error;
            return Status::error;
        };

        Packet() {
            setStatus(getPacketStatus());
        }

    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar & cereal::base_class<BasePacket>(this);
        }


        friend class cereal::access;

    public:
        const RoleList& requiredRoles() override {
            return Roles::getRequiredRolesDefinitions<S>();
        }

        ~Packet() override = default;

    protected:
        void _operate(PacketSourceWorker& f, std::shared_ptr<BasePacket> ptr) override {
            f.operate<S>(std::static_pointer_cast<S>(ptr));
        }

    };

//
//struct BaseGroup {
//    typedef BasePacket SendFileRequest;
//    typedef BasePacket SendFileResponse;
//    typedef BasePacket Error;
//    typedef BasePacket Ack;
//};
//
//template<typename Group=BaseGroup>
    struct PacketGroup {

        typedef Packet<PacketGroup, Packet<PacketGroup>, true, Status::request> Request;
        typedef Packet<PacketGroup, Packet<PacketGroup>, true, Status::response> Response;
        typedef Packet<PacketGroup, Packet<PacketGroup>, true, Status::error> Error;
        typedef Packet<PacketGroup, Packet<PacketGroup>, true, Status::ack> Ack;


    };

    template<typename PacketGroupType>
    using RequestPtr = typename PacketGroupType::Request::Ptr;
    template<typename PacketGroupType>
    using ResponsePtr = typename PacketGroupType::Response::Ptr;
    template<typename PacketGroupType>
    using AckPtr = typename PacketGroupType::Ack::Ptr;
    template<typename PacketGroupType>
    using ErrorPtr = typename PacketGroupType::Error::Ptr;

}


CEREAL_REGISTER_TYPE(bsc::PacketGroup::Request)
CEREAL_REGISTER_TYPE(bsc::PacketGroup::Response)
CEREAL_REGISTER_TYPE(bsc::PacketGroup::Error)
CEREAL_REGISTER_TYPE(bsc::PacketGroup::Ack)

#endif //BASYCO_PACKETINFO_H
