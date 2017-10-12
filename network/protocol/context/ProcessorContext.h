//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_PROCESSORCONTEXT_H
#define BASYCO_PROCESSORCONTEXT_H


#include <network/protocol/processor/ConnectionProcessor.h>
#include <network/protocol/packet/BasePacket.h>

class ProcessorContext {
private:
    ConnectionProcessor &processor;
    BasePacketPtr thisPacket = nullptr;

public:
    ConnectionProcessor &getProcessor() const;

    void setProcessor(ConnectionProcessor &processor);

    const BasePacketPtr &getThisPacket() const;

    void setThisPacket(const BasePacketPtr &thisPacket);

    ProcessorContext(ConnectionProcessor &processor);

};


#endif //BASYCO_PROCESSORCONTEXT_H
