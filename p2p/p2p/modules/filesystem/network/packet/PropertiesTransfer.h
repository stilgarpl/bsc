//
// Created by stilgar on 31.12.17.
//

#ifndef BSC_PROPERTIESTRANSFER_H
#define BSC_PROPERTIESTRANSFER_H

#include <core/utils/cereal_include.h>
#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <p2p/modules/filesystem/identification/TransferTypes.h>


namespace bsc {
    struct PropertiesTransfer : public PacketGroup {

    public:


        class Request : public bsc::Packet<PropertiesTransfer, PropertiesTransfer::Request> {
            TransferId transferId;
        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<PropertiesTransfer, PropertiesTransfer::Request>>(this), transferId);
            }


        public:
            TransferId getTransferId() const;

            void setTransferId(TransferId transferId);

        private:
            friend class cereal::access;
        };

        class Response : public bsc::Packet<PropertiesTransfer, PropertiesTransfer::Response> {
            TransferSize size;
        private:
            template<class Archive>
            void serialize(Archive& ar) {
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
}


CEREAL_REGISTER_TYPE(bsc::PropertiesTransfer::Request)
CEREAL_REGISTER_TYPE(bsc::PropertiesTransfer::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::PropertiesTransfer::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::PropertiesTransfer::Response)
#endif //BSC_PROPERTIESTRANSFER_H
