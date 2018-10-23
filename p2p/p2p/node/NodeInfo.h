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
#include "p2p/utils/cereal_include.h"


typedef std::string NodeIdType;
typedef std::string NetworkIdType;

class NodeInfo {

    NodeIdType nodeId;
    NetworkIdType networkId;
    ///@todo structure instead of string? we can store info like last successful connection to that address or sth
    std::set<std::string> knownAddresses;
    //typename Node::IdType id;
public:
    const std::string &getNetworkId() const;

    const std::set<std::string> &getKnownAddresses() const;
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
        knownAddresses.insert(address);
    }

    void printAll() const {
        std::cout << ":::NODE INFO:::" << std::endl
                  << "id : " << nodeId << std::endl
                  << "net id:" << networkId << std::endl;

    }

    void setNetworkId(const NetworkIdType &networkId);
};


#endif //BASYCO_NODEINFO_H
