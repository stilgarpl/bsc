//
// Created by stilgar on 06.09.17.
//

#ifndef BASYCO_GRAVITONPROTOCOL_H
#define BASYCO_GRAVITONPROTOCOL_H


#include <p2p/modules/network/protocol/logic/actions/BasePacketInfo.h>
#include "IProtocol.h"




class GravitonProtocol : public IProtocol {
private:
    std::mutex responseMapLock;
    std::map<BasePacket::IdType, std::shared_ptr<BasePacketInfo>> responseMap;
    //todo dynamic timeout
    const bsc::Tick::Clock::duration MAX_TIMEOUT = 1500ms;
    const unsigned int MAX_RETRY = 5;
public:
    explicit GravitonProtocol(bsc::LogicManager& logicManager);

    void onPacketSent(const PacketEvent &event) override;

    void onPacketReceived(const PacketEvent &event) override;

    void work(const bsc::Tick& tick) override;

    void onConnectionEvent(const ConnectionEvent &event) override;

    std::future<BasePacketPtr> send(Connection *conn, BasePacketPtr p, const Status &expectedStatus) override;


};


#endif //BASYCO_GRAVITONPROTOCOL_H
