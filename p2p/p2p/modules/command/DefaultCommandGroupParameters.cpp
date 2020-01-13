//
// Created by stilgar on 13.01.2020.
//

#include <core/io/InputOutputContext.h>
#include "DefaultCommandGroupParameters.h"

CommandModule::CommandExecutionStatus
defaultCommandGroupHandler(const DefaultCommandGroupParameters& params, const CommandModule::CommandGroup& group) {

    if (params.help().value_or(false) || params.arguments().empty()) {
        auto& io = Context::getActiveContext()->get<InputOutputContext>();
        //@todo text formatting.
        io.err() << "No commands given, listing available commands" << std::endl;
        for (const auto& item : group.getCommands()) {
            io.err() << " [ " << item << " ] " << std::endl;
        }
        return CommandModule::CommandExecutionStatus::noCommand;
    }

    return CommandModule::CommandExecutionStatus::success;

}
