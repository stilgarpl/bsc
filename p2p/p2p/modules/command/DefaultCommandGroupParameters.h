//
// Created by stilgar on 13.01.2020.
//

#ifndef BASYCO_DEFAULTCOMMANDGROUPPARAMETERS_H
#define BASYCO_DEFAULTCOMMANDGROUPPARAMETERS_H


#include <core/parameters/ProgramParameters.h>
#include "CommandModule.h"

struct DefaultCommandGroupParameters : ProgramParameters {
    Flag help = {'h', "help", "Shows help", false};
};

CommandModule::CommandExecutionStatus
defaultCommandGroupHandler(const DefaultCommandGroupParameters& params, const CommandModule::CommandGroup& group);


#endif //BASYCO_DEFAULTCOMMANDGROUPPARAMETERS_H
