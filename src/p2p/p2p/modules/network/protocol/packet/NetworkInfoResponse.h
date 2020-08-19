//
// Created by Krzysztof Tulidowicz on 06.10.17.
//

#ifndef BSC_NETWORKINFORESPONSE_H
#define BSC_NETWORKINFORESPONSE_H


#include "NetworkInfoGroup.h"
#include "p2p/modules/network/network/NetworkInfo.h"

namespace bsc {
    class NetworkInfoResponse : public bsc::Packet<NetworkInfoGroup, NetworkInfoResponse> {
    private:
        std::shared_ptr<NetworkInfo> networkInfo;
    public:
        const std::shared_ptr<NetworkInfo>& getNetworkInfo() const {
            return networkInfo;
        }

        void setNetworkInfo(const std::shared_ptr<NetworkInfo>& networkInfo) {
            NetworkInfoResponse::networkInfo = networkInfo;
        }

    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar & cereal::base_class<BasePacket>(this);
            ar & networkInfo;

        }

    public:

    private:


        friend class cereal::access;

    public:
        //    void process(Context::Ptr context) override;
    };
}

CEREAL_REGISTER_TYPE(bsc::NetworkInfoResponse)

#endif //BSC_NETWORKINFORESPONSE_H
