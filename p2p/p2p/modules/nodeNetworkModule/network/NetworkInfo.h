//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NETWORKINFO_H
#define BASYCO_NETWORKINFO_H


#include <list>
#include "p2p/node/NodeInfo.h"
#include "cereal/types/map.hpp"
#include "p2p/log/Logger.h"
#include "RemoteNodeInfo.h"
#include <optional>

//typedef std::string NetworkIdType;
typedef std::map<NodeIdType, RemoteNodeInfo::Ptr> NodesMapType;

class NetworkInfo {
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(networkId, knownNodes);
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


};


#endif //BASYCO_NETWORKINFO_H
