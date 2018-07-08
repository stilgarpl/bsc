//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_PROCESSORCONTEXT_H
#define BASYCO_PROCESSORCONTEXT_H


#include <p2p/network/protocol/processor/ConnectionProcessor.h>
#include <p2p/network/protocol/packet/BasePacket.h>

class ProcessorContext {
private:
    ConnectionProcessor &processor;
    BasePacketPtr thisPacket = nullptr;

public:
    ConnectionProcessor &getProcessor() const;

    const BasePacketPtr &getThisPacket() const;

    void setThisPacket(const BasePacketPtr &thisPacket);

    ProcessorContext(ConnectionProcessor &processor);

    //shortcut method for getting current packet from active context
    static const BasePacketPtr getCurrentPacketFromActiveContext() {
        //@todo not crash with no context?
        auto processor = Context::getActiveContext()->get<ProcessorContext>();
        if (processor != nullptr) {
            return processor->getThisPacket();
        } else {
            return nullptr;
        }

    }

};


#endif //BASYCO_PROCESSORCONTEXT_H
