//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NETWORKINFO_H
#define BASYCO_NETWORKINFO_H


#include <list>
#include "NodeInfo.h"
#include "cereal/types/map.hpp"
#include "../../log/Logger.h"
#include "experimental/optional"

//typedef std::string NetworkIdType;
typedef std::map<NodeIdType, NodeInfo> NodesMapType;

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

    NetworkInfo &operator+=(const NetworkInfo &other) {
        if (networkId == other.networkId) {
            ///@todo merge known nodes
            for (auto &&i : other.knownNodes) {
                addKnownNode(i.second);
            }
        }
    }

    void addKnownNode(const NodeInfo &nodeInfo);

    bool isNodeKnown(const NodeIdType &nodeId) {
        return knownNodes.count(nodeId) > 0;
    }



};


#endif //BASYCO_NETWORKINFO_H
