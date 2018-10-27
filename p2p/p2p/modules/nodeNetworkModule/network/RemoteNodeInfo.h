//
// Created by stilgar on 24.10.18.
//

#ifndef BASYCO_REMOTENODEINFO_H
#define BASYCO_REMOTENODEINFO_H


#include <p2p/node/NodeInfo.h>

class RemoteNodeInfo {

///@todo structure instead of string? we can store info like last successful connection to that address or sth
    std::set<std::string> knownAddresses;
    std::shared_ptr<NodeInfo> nodeInfo;
public:
    const std::set<std::string> &getKnownAddresses() const;

    void addKnownAddress(std::string address) {
        knownAddresses.insert(address);
    }

//    RemoteNodeInfo& operator=(const NodeInfo& ni) {
//        this->setNodeId(ni.getNodeId());
//        this->setNetworkId(ni.getNetworkId());
//        return *this;
//    }

    const std::shared_ptr<NodeInfo> &getNodeInfo() const;

    void setNodeInfo(const std::shared_ptr<NodeInfo> &nodeInfo);

};


#endif //BASYCO_REMOTENODEINFO_H
