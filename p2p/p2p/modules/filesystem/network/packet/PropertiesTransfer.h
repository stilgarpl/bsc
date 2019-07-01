//
// Created by stilgar on 31.12.17.
//

#ifndef BASYCO_PROPERTIESTRANSFER_H
#define BASYCO_PROPERTIESTRANSFER_H

#include <core/utils/cereal_include.h>
#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <p2p/modules/filesystem/identification/TransferTypes.h>

struct PropertiesTransfer : public PacketGroup {

public:


    class Request : public Packet<PropertiesTransfer, PropertiesTransfer::Request> {
        TransferId transferId;
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<PropertiesTransfer, PropertiesTransfer::Request>>(this), transferId);
        }

    public:
        void process(Context::Ptr context) override;

    private:
    public:
        TransferId getTransferId() const;

        void setTransferId(TransferId transferId);

    private:
        friend class cereal::access;
    };

    class Response : public Packet<PropertiesTransfer, PropertiesTransfer::Response> {
        TransferSize size;
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<PropertiesTransfer, PropertiesTransfer::Response>>(this), size);
        }

    public:
        TransferSize getSize() const {
            return size;
        }

        void setSize(TransferSize size) {
            Response::size = size;
        }

    private:
        friend class cereal::access;

    };
};


CEREAL_REGISTER_TYPE(PropertiesTransfer::Request)
CEREAL_REGISTER_TYPE(PropertiesTransfer::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, PropertiesTransfer::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, PropertiesTransfer::Response)


#endif //BASYCO_PROPERTIESTRANSFER_H
