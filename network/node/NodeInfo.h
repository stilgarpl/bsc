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
    std::list<std::string> knownAddresses;
public:
    const std::string &getNodeName() const;

    void setNodeName(const std::string &nodeName);

};


#endif //BASYCO_NODEINFO_H
