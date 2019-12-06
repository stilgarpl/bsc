//
// Created by stilgar on 19.11.17.
//

#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/command/CommandModule.h>
#include <p2p/modules/command/network/packet/CommandPacket.h>
#include <core/io/InputOutputContext.h>
#include <p2p/modules/command/CommandInputOutputContext.h>
#include "CommandActions.h"

void CommandActions::runRemoteCommand(const CommandEvent& commandEvent) {
    LOGGER("remote command!")
    auto connectionContext = Context::getActiveContext()->get<ConnectionContext>();
    auto nc = Context::getActiveContext()->get<NodeContext>();
    if (nc != nullptr && connectionContext != nullptr) {
        Connection& connection = connectionContext->getConnection();//commandEvent.origin();
        auto commandModule = nc->getNode().getModule<CommandModule>();
        //todo this action shouldn't be static and have commandmodule as a field setDirect from it's constructor. or it can be a lambda in CommandModule logic.
        if (commandModule != nullptr) {
            //setting up remote command context.
            Context::OwnPtr remoteCommandContext = Context::makeContext(Context::getActiveContext());
            auto ioContext = std::make_shared<CommandInputOutputContext>();
            remoteCommandContext->setDirect<InputOutputContext>(ioContext);
            {
                SetLocalContext localContext(remoteCommandContext);
                bool runStatus = commandModule->runCommand(commandEvent.getCommandName(), commandEvent.getData());
                CommandPacket::Response::Ptr res = CommandPacket::Response::getNew(commandEvent.getRequestId());
                res->setRunStatus(runStatus);
                res->setOutput(ioContext->getOutputStream().str());
                connection.send(res);
            }
        }

    } else {
        LOGGER("NO NODE CONTEXT")
    }

}
