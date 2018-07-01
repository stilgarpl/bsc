//
// Created by stilgar on 01.07.18.
//

#ifndef BASYCO_NODENETWORKLOGICSOURCE_H
#define BASYCO_NODENETWORKLOGICSOURCE_H


#include <p2p/logic/sources/EventQueueSource.h>
#include <p2p/modules/nodeNetworkModule/hll/events/NodeNetworkLogicEvent.h>
#include <p2p/logic/HighLevelLogicSource.h>

class NodeNetworkLogicSource : public HighLevelLogicSource /*public EventQueueSource<NodeNetworkLogicEvent> */{


    void defineLogic() {
//        when(ConnectionEvent::IdType::CONNECTION_ESTABLISHED);
//        when(any<ConnectionEvent>())
    }

};


#endif //BASYCO_NODENETWORKLOGICSOURCE_H
