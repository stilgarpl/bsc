//
// Created by stilgar on 06.09.17.
//

#ifndef BASYCO_GRAVITONPROTOCOL_H
#define BASYCO_GRAVITONPROTOCOL_H


#include <network/protocol/logic/actions/NetworkPacketInfo.h>
#include "IProtocol.h"


class GravitonProtocol : public IProtocol {
private:
    std::mutex lock;
    std::map<BasePacket::IdType, std::shared_ptr<NetworkPacketInfo>> responseMap;
public:
    void onPacketSent(const PacketEvent &event) override;

    void onPacketReceived(const PacketEvent &event) override;

    void work(const Tick &tick) override;

    std::future<BasePacketPtr> send(Connection *conn, BasePacketPtr p) override;
};


#endif //BASYCO_GRAVITONPROTOCOL_H
