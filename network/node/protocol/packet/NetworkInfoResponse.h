//
// Created by stilgar on 06.10.17.
//

#ifndef BASYCO_NETWORKINFORESPONSE_H
#define BASYCO_NETWORKINFORESPONSE_H


#include "../../../protocol/packet/BasePacket.h"
#include "../../NetworkInfo.h"

class NetworkInfoResponse : public BasePacket {
private:
    std::shared_ptr<NetworkInfo> networkInfo;
public:
    const std::shared_ptr<NetworkInfo> &getNetworkInfo() const {
        return networkInfo;
    }

    void setNetworkInfo(const std::shared_ptr<NetworkInfo> &networkInfo) {
        NetworkInfoResponse::networkInfo = networkInfo;
    }

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & cereal::base_class<BasePacket>(this);
        ar & networkInfo;

    }

public:
    NetworkInfoResponse();

private:


    friend class cereal::access;

public:
    void process(Context &context) override;
};

CEREAL_REGISTER_TYPE(NetworkInfoResponse);

#endif //BASYCO_NETWORKINFORESPONSE_H
