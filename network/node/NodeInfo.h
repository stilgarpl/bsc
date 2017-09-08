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

class NodeInfo {

    std::string nodeName;
    std::string networkId;
    std::list<std::string> knownAddresses;
    //typename Node::IdType id;
public:
    const std::string &getNodeName() const;

    void setNodeName(const std::string &nodeName);

    const std::string &getNetworkId() const;

    const std::list<std::string> &getKnownAddresses() const;
//
//    Node::IdType getId() const;
//
//    void setId(Node::IdType id);

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(nodeName, networkId, knownAddresses);
    }


    friend class cereal::access;

};


#endif //BASYCO_NODEINFO_H
