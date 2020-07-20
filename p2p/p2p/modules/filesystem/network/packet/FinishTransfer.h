//
// Created by Krzysztof Tulidowicz on 30.12.17.
//

#ifndef BSC_FINISHTRANSFER_H
#define BSC_FINISHTRANSFER_H


#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <p2p/modules/filesystem/identification/TransferTypes.h>


namespace bsc {
    struct FinishTransfer : public PacketGroup {

    public:


        class Request : public bsc::Packet<FinishTransfer, FinishTransfer::Request> {
            TransferId transferId;
        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<FinishTransfer, FinishTransfer::Request>>(this), transferId);
            }

        public:
            TransferId getTransferId() const {
                return transferId;
            }

            void setTransferId(TransferId transferId) {
                Request::transferId = transferId;
            }


        private:
            friend class cereal::access;
        };

        class Response : public bsc::Packet<FinishTransfer, FinishTransfer::Response> {
        private:
            template<class Archive>
            void serialize(Archive& ar) {
                ar(cereal::base_class<Packet<FinishTransfer, FinishTransfer::Response>>(this));
            }


            friend class cereal::access;

        };
    };
}


CEREAL_REGISTER_TYPE(bsc::FinishTransfer::Request)
CEREAL_REGISTER_TYPE(bsc::FinishTransfer::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::FinishTransfer::Request)

CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::BasePacket, bsc::FinishTransfer::Response)
#endif //BSC_FINISHTRANSFER_H
