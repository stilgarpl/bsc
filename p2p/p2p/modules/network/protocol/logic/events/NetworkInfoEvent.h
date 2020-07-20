//
// Created by Krzysztof Tulidowicz on 06.10.17.
//

#ifndef BSC_NETWORKINFOEVENT_H
#define BSC_NETWORKINFOEVENT_H


#include <p2p/modules/network/network/NetworkInfo.h>
#include "logic/IEvent.h"


namespace bsc {
    enum class NetworInfoEventId {
        NETWORK_INFO_RECEIVED,
        SAVE_NETWORK_INFO,

    };

    class NetworkInfoEvent : public bsc::IEvent<NetworInfoEventId> {
    private:
        NetworkInfo networkInfo;

    public:
        const NetworkInfo& getNetworkInfo() const;

        void setNetworkInfo(const NetworkInfo& networkInfo);
    };


}


#endif //BSC_NETWORKINFOEVENT_H
