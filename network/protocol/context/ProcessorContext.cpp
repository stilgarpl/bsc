//
// Created by stilgar on 12.10.17.
//

#include "ProcessorContext.h"

ConnectionProcessor &ProcessorContext::getProcessor() const {
    return processor;
}


const BasePacketPtr &ProcessorContext::getThisPacket() const {
    return thisPacket;
}

void ProcessorContext::setThisPacket(const BasePacketPtr &thisPacket) {
    ProcessorContext::thisPacket = thisPacket;
}

ProcessorContext::ProcessorContext(ConnectionProcessor &processor) : processor(processor) {}
