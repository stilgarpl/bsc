//
// Created by stilgar on 06.09.17.
//

#ifndef BSC_GRAVITONPROTOCOL_H
#define BSC_GRAVITONPROTOCOL_H


#include <p2p/modules/network/protocol/logic/actions/BasePacketInfo.h>
#include "IProtocol.h"


namespace bsc {
    class GravitonProtocol : public IProtocol {
    private:
        std::mutex responseMapLock;
        std::map<BasePacket::IdType, std::shared_ptr<BasePacketInfo>> responseMap;
        //todo dynamic timeout
        const bsc::Tick::Clock::duration MAX_TIMEOUT = 1500s;
        const unsigned int MAX_RETRY = 5;
    public:
        explicit GravitonProtocol(LogicManager& logicManager);

        void onPacketSent(const PacketEvent& event) override;

        void onPacketReceived(const PacketEvent& event) override;

        void work(const Tick& tick) override;

        void onConnectionEvent(const ConnectionEvent& event) override;

        std::future<BasePacketPtr> send(Connection* conn, BasePacketPtr p, const Status& expectedStatus) override;


    };
}


#endif //BSC_GRAVITONPROTOCOL_H
