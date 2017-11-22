//
// Created by stilgar on 06.09.17.
//

#ifndef BASYCO_GRAVITONPROTOCOL_H
#define BASYCO_GRAVITONPROTOCOL_H


#include <p2p/network/protocol/logic/actions/BasePacketInfo.h>
#include "IProtocol.h"



class GravitonProtocol : public IProtocol {
private:
    std::mutex lock;
    std::map<BasePacket::IdType, std::shared_ptr<BasePacketInfo>> responseMap;
    const Tick::clock::duration MAX_TIMEOUT = 9500ms;
public:
    void onPacketSent(const PacketEvent &event) override;

    void onPacketReceived(const PacketEvent &event) override;

    void work(const Tick &tick) override;

    void onConnectionEvent(const ConnectionEvent &event) override;

    std::future<BasePacketPtr> send(Connection *conn, BasePacketPtr p, const Status &expectedStatus) override;
};


#endif //BASYCO_GRAVITONPROTOCOL_H
