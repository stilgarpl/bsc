//
// Created by Krzysztof Tulidowicz on 19.07.18.
//

#ifndef BSC_AUTHHELLOEVENT_H
#define BSC_AUTHHELLOEVENT_H


#include <p2p/core/node/NodeInfo.h>
#include <logic/IEvent.h>


namespace bsc {
    class AuthHelloEvent : public IEvent<int> {
        NodeIdType nodeId;
        std::string authData;

    public:
        [[nodiscard]] const NodeIdType& getNodeId() const;

        void setNodeId(const NodeIdType& nodeId);

        [[nodiscard]] const std::string& getAuthData() const;

        void setAuthData(const std::string& authData);
    };
}


#endif //BSC_AUTHHELLOEVENT_H