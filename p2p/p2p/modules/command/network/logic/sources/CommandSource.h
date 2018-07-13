//
// Created by stilgar on 19.11.17.
//

#ifndef BASYCO_COMMANDSOURCE_H
#define BASYCO_COMMANDSOURCE_H


#include <p2p/logic/sources/EventQueueSource.h>
#include <p2p/modules/command/network/logic/events/CommandEvent.h>

class CommandSource : public EventQueueSource<CommandEvent> {
public:

    CommandSource(SourceManager &sourceManager);

    void commandReceived(const BasePacket::IdType &id, const std::vector<std::string> &modules,
                         const std::string &commandName,
                         const std::vector<std::string> &data);
};


#endif //BASYCO_COMMANDSOURCE_H
