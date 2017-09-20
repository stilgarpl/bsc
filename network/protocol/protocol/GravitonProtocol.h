//
// Created by stilgar on 06.09.17.
//

#ifndef BASYCO_GRAVITONPROTOCOL_H
#define BASYCO_GRAVITONPROTOCOL_H


#include "IProtocol.h"
#include "../logic/actions/NetworkPacketInfo.h"

class GravitonProtocol : public IProtocol {
private:
    std::map<NetworkPacket::IdType, std::shared_ptr<NetworkPacketInfo>> responseMap;
public:
    void onPacketSent(const PacketEvent &event) override;

    void onPacketReceived(const PacketEvent &event) override;

    void work(const Tick &tick) override;

    std::future<NetworkPacketPtr> send(Connection *conn, NetworkPacketPtr p) override;
};


#endif //BASYCO_GRAVITONPROTOCOL_H
