//
// Created by stilgar on 19.11.17.
//

#include "CommandSource.h"

void CommandSource::commandReceived(const BasePacket::IdType &id,
                                    const std::string &commandName, const std::vector<std::string> &data) {
    auto event = newEvent();
    event->setEventId(EventType::IdType::EXECUTE_COMMAND);
    event->setCommandName(commandName);
    event->setData(data);
//        event->setRequestId(0);

    queueEvent(event);
}

CommandSource::CommandSource(SourceManager &sourceManager) : EventQueueSource(sourceManager) {}
