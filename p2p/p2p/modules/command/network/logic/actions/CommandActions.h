//
// Created by stilgar on 19.11.17.
//

#ifndef BSC_COMMANDACTIONS_H
#define BSC_COMMANDACTIONS_H


#include <p2p/modules/command/network/logic/events/CommandEvent.h>


namespace bsc {
    class CommandActions {
    public:
        enum Actions {
            RUN_COMMAND,
        };


        static void runRemoteCommand(const CommandEvent& commandEvent);

    };
}


#endif //BSC_COMMANDACTIONS_H
