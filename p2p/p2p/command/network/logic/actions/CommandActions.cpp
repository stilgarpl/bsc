//
// Created by stilgar on 19.11.17.
//

#include <p2p/network/protocol/context/NodeContext.h>
#include <p2p/network/node/modules/CommandModule.h>
#include "CommandActions.h"

void CommandActions::runCommand(const CommandEvent &commandEvent) {
    auto nc = Context::getActiveContext().get<NodeContext>();
    if (nc != nullptr) {
        auto commandModule = nc->getNode().getModule<CommandModule>();
        if (commandModule != nullptr) {
            if (commandEvent.getModules().size() == 0) {
                commandModule->runCommand(commandEvent.getCommandName(), commandEvent.getData());
            } else {
                CommandModule::CommandSubModule *pSubModule = &commandModule->submodule(commandEvent.getModules()[0]);
                for (unsigned int i = 1/* 1, we already have the 0 */; i < commandEvent.getModules().size(); ++i) {
                    pSubModule = &pSubModule->submodule(commandEvent.getModules()[i]);
                }
                pSubModule->runCommand(commandEvent.getCommandName(), commandEvent.getData());

            }
        }

    }

}