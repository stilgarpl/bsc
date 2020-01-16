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


namespace bsc {
    //typedef std::string NetworkIdType;
    typedef std::map<NodeIdType, RemoteNodeInfo::Ptr> NodesMapType;

    class NetworkInfo {
    private:
        mutable std::recursive_mutex networkInfoLock;

        template<class Archive>
        void serialize(Archive& ar) {
            std::unique_lock g(networkInfoLock);
            ar(CEREAL_NVP(networkId), CEREAL_NVP(knownNodes));
        }


        friend class cereal::access;

    private:
        //@todo turn into map?
        NetworkIdType networkId;
        NodesMapType knownNodes;

    public:
        const NetworkIdType& getNetworkId() const;

        void setNetworkId(const NetworkIdType& networkId);

        NetworkInfo& operator+=(const NetworkInfo& other);

        void addKnownNode(const NodeInfo& nodeInfo);

        bool isNodeKnown(const NodeIdType& nodeId);

        NodeInfo& getNodeInfo(const NodeIdType& nodeId);

        RemoteNodeInfo& getRemoteNodeInfo(const NodeIdType& nodeId);

        void addKnownAddress(const NodeIdType& nodeId, const NetAddressType& address);

        NetworkInfo& operator=(const NetworkInfo& rhs) {
            std::unique_lock g(networkInfoLock);
            if (this != &rhs) {
                this->networkId = rhs.networkId;
                this->knownNodes = rhs.knownNodes;
            }
            return *this;
        }

        NetworkInfo() = default;

        NetworkInfo(const NetworkInfo& rhs) {
            this->networkId = rhs.networkId;
            this->knownNodes = rhs.knownNodes;
        }


    };
}


#endif //BASYCO_NETWORKINFO_H
