//
// Created by stilgar on 31.12.17.
//

#ifndef BASYCO_DATATRANSFER_H
#define BASYCO_DATATRANSFER_H


#include <core/utils/cereal_include.h>
#include <p2p/modules/network/protocol/packet/info/PacketInfo.h>
#include <p2p/modules/filesystem/identification/TransferTypes.h>

struct DataTransfer : public PacketGroup {

public:


    class Request : public Packet<DataTransfer, DataTransfer::Request> {
        TransferId transferId;
        TransferSize begin, end;
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<DataTransfer, DataTransfer::Request >>(this), transferId, begin, end);
        }


    private:
        friend class cereal::access;

    public:
        TransferId getTransferId() const;

        void setTransferId(TransferId transferId);

        TransferSize getBegin() const;

        void setBegin(TransferSize begin);

        TransferSize getEnd() const;

        void setEnd(TransferSize end);
    };

    class Response : public Packet<DataTransfer, DataTransfer::Response> {
        RawDataType data;
        TransferSize begin, end;
    private:
        template<class Archive>
        void serialize(Archive &ar) {
            ar(cereal::base_class<Packet<DataTransfer, DataTransfer::Response >>(this), data, begin, end);
        }

    public:
        RawDataType &getData() {
            return data;
        }

        void setData(const RawDataType &data) {
            Response::data = data;
        }

        TransferSize getBegin() const {
            return begin;
        }

        void setBegin(TransferSize begin) {
            Response::begin = begin;
        }

        TransferSize getEnd() const {
            return end;
        }

        void setEnd(TransferSize end) {
            Response::end = end;
        }

    private:
        friend class cereal::access;

    };
};


CEREAL_REGISTER_TYPE(DataTransfer::Request)
CEREAL_REGISTER_TYPE(DataTransfer::Response)

CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, DataTransfer::Request)
CEREAL_REGISTER_POLYMORPHIC_RELATION(BasePacket, DataTransfer::Response)


#endif //BASYCO_DATATRANSFER_H
