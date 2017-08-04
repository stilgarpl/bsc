//
// Created by stilgar on 31.07.17.
//

#ifndef BASYCO_NODEINFO_H
#define BASYCO_NODEINFO_H


#include <string>

#include <list>
#include <Poco/Net/SocketAddress.h>

class NodeInfo {

    std::string nodeName;
    std::string networkId;
    std::list<std::string> knownAddresses;
public:
    const std::string &getNodeName() const;

    void setNodeName(const std::string &nodeName);

    const std::string &getNetworkId() const;

    const std::list<std::string> &getKnownAddresses() const;

};


#endif //BASYCO_NODEINFO_H
