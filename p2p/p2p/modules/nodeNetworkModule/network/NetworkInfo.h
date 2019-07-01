//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NETWORKINFO_H
#define BASYCO_NETWORKINFO_H


#include <list>
#include "cereal/types/map.hpp"
#include "core/log/Logger.h"
#include "RemoteNodeInfo.h"
#include <optional>


//typedef std::string NetworkIdType;
typedef std::map<NodeIdType, RemoteNodeInfo::Ptr> NodesMapType;

class NetworkInfo {
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(networkId), CEREAL_NVP(knownNodes));
    }


    friend class cereal::access;
private:
    //@todo turn into map?
    NetworkIdType networkId;
    NodesMapType knownNodes;

public:
    const NetworkIdType &getNetworkId() const;

    void setNetworkId(const NetworkIdType &networkId);

    NetworkInfo &operator+=(const NetworkInfo &other);

    void addKnownNode(const NodeInfo &nodeInfo);

    bool isNodeKnown(const NodeIdType &nodeId);

    NodeInfo &getNodeInfo(const NodeIdType &nodeId);

    RemoteNodeInfo &getRemoteNodeInfo(const NodeIdType &nodeId);

    void addKnownAddress(const NodeIdType &nodeId, const NetAddressType &address);


};


#endif //BASYCO_NETWORKINFO_H
