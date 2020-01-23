//
// Created by stilgar on 24.10.18.
//

#ifndef BSC_REMOTENODEINFO_H
#define BSC_REMOTENODEINFO_H


#include <p2p/node/NodeInfo.h>
#include <p2p/modules/network/network/NetAddressType.h>
#include <atomic>
#include <mutex>


namespace bsc {
    class RemoteNodeInfo {

        mutable std::mutex nodeInfoLock;

        //@todo C++20 std::atomic<std::shared_ptr<NodeInfo>>
        std::shared_ptr<NodeInfo> nodeInfo;
        ///@todo structure instead of string? we can store info like last successful connection to that address or sth
        std::set<NetAddressType> knownAddresses;

    public:
        [[nodiscard]] const std::set<NetAddressType>& getKnownAddresses() const;

        void addKnownAddress(const NetAddressType& address) {
            std::unique_lock g(nodeInfoLock);
            knownAddresses.insert(address);
        }

        RemoteNodeInfo& operator=(const RemoteNodeInfo& ni) {
            std::unique_lock g(nodeInfoLock);
            //@todo I don't like this method. See who uses it and why. Maybe remove it.
            if (this != &ni) {
                this->nodeInfo = ni.nodeInfo;
                this->knownAddresses = ni.knownAddresses;
            }
            return *this;
        }

        [[nodiscard]] std::shared_ptr<NodeInfo> getNodeInfo() const;

        void setNodeInfo(const std::shared_ptr<NodeInfo>& nodeInfo);

    private:
        template<class Archive>
        void serialize(Archive& ar) {
            std::unique_lock g(nodeInfoLock);
            ar & nodeInfo;
            ar & knownAddresses;
        }


        friend class cereal::access;

    public:
        typedef std::shared_ptr<RemoteNodeInfo> Ptr;

    };
}


#endif //BSC_REMOTENODEINFO_H
