//
// Created by stilgar on 19.11.17.
//

#ifndef BASYCO_COMMANDACTIONS_H
#define BASYCO_COMMANDACTIONS_H


#include <p2p/command/network/logic/events/CommandEvent.h>


class CommandActions {
public:
    enum Actions {
        RUN_COMMAND,
    };


    static void runCommand(const CommandEvent &commandEvent);

};


#endif //BASYCO_COMMANDACTIONS_H
