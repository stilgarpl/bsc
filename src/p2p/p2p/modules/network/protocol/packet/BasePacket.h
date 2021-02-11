//
// Created by Krzysztof Tulidowicz on 01.08.17.
//

#ifndef BSC_BASEPACKET_H
#define BSC_BASEPACKET_H

#include <core/utils/cereal_include.h>
#include <log/log/Logger.h>
#include <p2p/modules/network/protocol/enum/Status.h>

#include <context/context/Context.h>
#include <p2p/core/role/Roles.h>

namespace bsc {

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
        void serialize(Archive& ar) {
            ar(status, id, retry);
        }

        friend class cereal::access;

    public:


        [[nodiscard]] Status getStatus() const;

        void setStatus(Status status);

        [[nodiscard]] IdType getId() const;

        void setId(IdType id);

        //I don't remember why it was deleted, probably because of serialization bug(fixed) or maybe to not waste ids (who cares about ids?)
        // BasePacket(const BasePacket &) = delete;

        BasePacket() : status(Status::request), id(nextId()) {};

        [[nodiscard]] bool isRetry() const;

        void setRetry(bool retry);

        virtual const RoleList& requiredRoles() = 0;

        void resetId() {
            setId(nextId());
        }


    protected:

        virtual void _operate(PacketSourceWorker& f, std::shared_ptr<BasePacket>) = 0;

        friend class ConnectionSource;

        virtual ~BasePacket() = default;
    };

    typedef std::shared_ptr<bsc::BasePacket> BasePacketPtr;


    template<typename... T1>
    using Fu = std::function<void(T1& ...)>;


}


CEREAL_REGISTER_TYPE(bsc::BasePacket)

#endif //BSC_BASEPACKET_H
