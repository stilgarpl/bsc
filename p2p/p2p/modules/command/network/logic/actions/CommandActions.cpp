//
// Created by stilgar on 19.11.17.
//

#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/command/CommandModule.h>
#include <p2p/modules/command/network/packet/CommandPacket.h>
#include "CommandActions.h"

void CommandActions::runCommand(const CommandEvent &commandEvent) {
    LOGGER("remote command!")
    Connection *connection = commandEvent.origin();
    auto nc = Context::getActiveContext()->get<NodeContext>();
    if (nc != nullptr) {
        auto commandModule = nc->getNode().getModule<CommandModule>();
        if (commandModule != nullptr) {
            bool runStatus = false;
            if (commandEvent.getModules().size() == 0) {
                runStatus = commandModule->runCommand(commandEvent.getCommandName(), commandEvent.getData());
            } else {
                CommandModule::CommandSubModule *pSubModule = &commandModule->submodule(commandEvent.getModules()[0]);
                for (unsigned int i = 1/* 1, we already have the 0 */; i < commandEvent.getModules().size(); ++i) {
                    pSubModule = &pSubModule->submodule(commandEvent.getModules()[i]);
                }
                runStatus = pSubModule->runCommand(commandEvent.getCommandName(), commandEvent.getData());

            }
            CommandPacket::Response::Ptr res = CommandPacket::Response::getNew(commandEvent.getRequestId());
            res->setRunStatus(runStatus);
            connection->send(res);
        }

    } else {
        LOGGER("NO NODE CONTEXT")
    }

}
