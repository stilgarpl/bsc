//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NODEINFO_H
#define BASYCO_NODEINFO_H


#include <string>

#include <list>
#include <Poco/Net/SocketAddress.h>
#include <cereal/access.hpp>
//#include "Node.h"

#include "../../utils/cereal_include.h"


typedef std::string NodeIdType;
typedef std::string NetworkIdType;

class NodeInfo {

    NodeIdType nodeId;
    NetworkIdType networkId;
    std::list<std::string> knownAddresses;
    //typename Node::IdType id;
public:
    const std::string &getNetworkId() const;

    const std::list<std::string> &getKnownAddresses() const;
//
//    Node::IdType getId() const;
//
//    void setId(Node::IdType id);

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(nodeId, networkId, knownAddresses);
    }


    friend class cereal::access;

public:
    const NodeIdType &getNodeId() const;

    void setNodeId(const NodeIdType &nodeId);

    void addKnownAddress(std::string address) {
        knownAddresses.push_back(address);
    }
};


#endif //BASYCO_NODEINFO_H
