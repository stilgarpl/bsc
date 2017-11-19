//
// Created by stilgar on 19.11.17.
//

#ifndef BASYCO_COMMANDSOURCE_H
#define BASYCO_COMMANDSOURCE_H


#include <p2p/logic/sources/EventQueueSource.h>
#include <p2p/command/network/logic/events/CommandEvent.h>

class CommandSource : public EventQueueSource<CommandEvent> {
public:

    void commandReceived(const std::vector<std::string> &modules, const std::string &commandName,
                         const std::vector<std::string> &data) {
        auto event = newEvent();
        event->setCommandName(commandName);
        event->setModules(modules);
        event->setData(data);

        queueEvent(event);
    }
};


#endif //BASYCO_COMMANDSOURCE_H
