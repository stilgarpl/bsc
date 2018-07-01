//
// Created by stilgar on 19.11.17.
//

#include "CommandSource.h"

void CommandSource::commandReceived(const BasePacket::IdType &id, const std::vector<std::string> &modules,
                                    const std::string &commandName, const std::vector<std::string> &data) {
    auto event = newEvent();
    event->setCommandName(commandName);
    event->setModules(modules);
    event->setData(data);
//        event->setRequestId(0);

    queueEvent(event);
}

CommandSource::CommandSource(SourceManager &sourceManager) : EventQueueSource(sourceManager) {}
