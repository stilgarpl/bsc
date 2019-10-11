//
// Created by stilgar on 24.10.18.
//

#ifndef BASYCO_REMOTENODEINFO_H
#define BASYCO_REMOTENODEINFO_H


#include <p2p/node/NodeInfo.h>
#include <p2p/modules/network/network/NetAddressType.h>
#include <atomic>
class RemoteNodeInfo {

    //@todo C++20 std::atomic<std::shared_ptr<NodeInfo>>
    std::shared_ptr<NodeInfo> nodeInfo;
///@todo structure instead of string? we can store info like last successful connection to that address or sth
    std::set<NetAddressType> knownAddresses;

public:
    [[nodiscard]] const std::set<NetAddressType>& getKnownAddresses() const;

    void addKnownAddress(const NetAddressType &address) {
        knownAddresses.insert(address);
    }

//    RemoteNodeInfo& operator=(const NodeInfo& ni) {
//        this->setNodeId(ni.getNodeId());
//        this->setNetworkId(ni.getNetworkId());
//        return *this;
//    }

    [[nodiscard]] std::shared_ptr<NodeInfo> getNodeInfo() const;

    void setNodeInfo(const std::shared_ptr<NodeInfo> &nodeInfo);

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & nodeInfo;
        ar & knownAddresses;
    }


    friend class cereal::access;

public:
    typedef std::shared_ptr<RemoteNodeInfo> Ptr;

};




#endif //BASYCO_REMOTENODEINFO_H
