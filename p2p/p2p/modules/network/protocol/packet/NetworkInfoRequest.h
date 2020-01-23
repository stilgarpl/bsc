//
// Created by stilgar on 06.10.17.
//

#ifndef BSC_NETWORKINFOREQUEST_H
#define BSC_NETWORKINFOREQUEST_H


#include "NetworkInfoGroup.h"


namespace bsc {
    class NetworkInfoRequest : public bsc::Packet<NetworkInfoGroup, NetworkInfoRequest> {
    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar & cereal::base_class<BasePacket>(this);

        }

    private:

        friend class cereal::access;

    };
}

CEREAL_REGISTER_TYPE(bsc::NetworkInfoRequest)

#endif //BSC_NETWORKINFOREQUEST_H
