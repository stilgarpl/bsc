//
// Created by Krzysztof Tulidowicz on 06.09.17.
//

#ifndef BSC_NETWORKPACKETINFO_H
#define BSC_NETWORKPACKETINFO_H

#include <future>
#include <p2p/modules/network/protocol/packet/BasePacket.h>
#include <p2p/modules/network/protocol/connection/Connection.h>
#include "logic/events/Tick.h"


namespace bsc {
    class BasePacketInfo {
    private:
        unsigned int retry = 0;
        BasePacketPtr packetPtr;
        Connection* connection = nullptr;
        //@todo is Tick reference necessary? maybe some global Clock function? or just std::steady_clock ?
        Status expectedStatus = Status::response; //@todo what if we expect more than one response? error for example?
        bsc::Tick::Clock::time_point timeSent;
        std::promise<BasePacketPtr> responsePromise;

    public:
        BasePacketInfo(const BasePacketPtr& packetPtr, Connection* connection,
                       const std::chrono::time_point<Tick::Clock>& timeSent);

    public:
        const BasePacketPtr& getPacketPtr() const;

        void setPacketPtr(const BasePacketPtr& packetPtr);

        const std::chrono::time_point<Tick::Clock>& getTimeSent() const;

        void setTimeSent(const std::chrono::time_point<Tick::Clock>& timeSent);

    public:
        BasePacketInfo(const BasePacketPtr& packetPtr, const std::chrono::time_point<Tick::Clock>& timeSent);

        BasePacketInfo(const BasePacketPtr& packetPtr, Connection* connection,
                       const std::chrono::time_point<Tick::Clock>& timeSent, Status expectedStatus);

        Connection* getConnection() const;

        void setConnection(Connection* connection);

        //@todo or simply get future?
        std::promise<BasePacketPtr>& getResponsePromise();

        Status getExpectedStatus() const;

        void setExpectedStatus(Status expectedStatus);

        unsigned int getRetry() const;

        void setRetry(unsigned int retry);

    };
}


#endif //BSC_NETWORKPACKETINFO_H
