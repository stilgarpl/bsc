//
// Created by stilgar on 08.09.17.
//

#include "NodeSource.h"


void bsc::NodeSource::nodeInfoReceived(const bsc::NodeInfo& nodeInfo) {
    auto event = newEvent();
    event->setNodeInfo(nodeInfo);
    event->setEventId(EventType::IdType::NODE_INFO_RECEIVED);

    queueEvent(event);
}

void bsc::NodeSource::nodeDiscovered(const bsc::NodeInfo& nodeInfo) {
    auto event = newEvent();
    event->setNodeInfo(nodeInfo);
    event->setEventId(EventType::IdType::NEW_NODE_DISCOVERED);

    queueEvent(event);
}

bsc::NodeSource::NodeSource(bsc::SourceManager& sourceManager) : EventQueueSource(sourceManager) {}
