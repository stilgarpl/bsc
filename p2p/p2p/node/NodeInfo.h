//
// Created by stilgar on 31.07.17.
//

#ifndef BSC_NODEINFO_H
#define BSC_NODEINFO_H


#include <string>
#include <iostream>
#include <core/utils/cereal_include.h>


namespace bsc {
    typedef std::string NodeIdType;
    typedef std::string NetworkIdType;

    class NodeInfo {

        NodeIdType nodeId;
        NetworkIdType networkId;
        //typename Node::IdType id;
    public:
        const std::string& getNetworkId() const;

        //
        //    Node::IdType getId() const;
        //
        //    void setId(Node::IdType id);

    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar(nodeId, networkId);
        }


        friend class cereal::access;

    public:
        const NodeIdType& getNodeId() const;

        void setNodeId(const NodeIdType& nodeId);

        void printAll() const {
            std::cout << ":::NODE INFO:::" << std::endl
                      << "id : " << nodeId << std::endl
                      << "net id:" << networkId << std::endl;

        }

        void setNetworkId(const NetworkIdType& networkId);
    };
}


#endif //BSC_NODEINFO_H
