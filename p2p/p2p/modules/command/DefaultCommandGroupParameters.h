//
// Created by stilgar on 13.01.2020.
//

#ifndef BASYCO_DEFAULTCOMMANDGROUPPARAMETERS_H
#define BASYCO_DEFAULTCOMMANDGROUPPARAMETERS_H


#include <parser/parameters/CommandLineParameters.h>
#include "CommandModule.h"


namespace bsc {
    struct DefaultCommandGroupParameters : bsc::CommandLineParameters {
        Flag help = {'h', "help", "Shows help", false};
    };

    CommandModule::CommandExecutionStatus
    defaultCommandGroupHandler(const bsc::DefaultCommandGroupParameters& params,
                               const CommandModule::CommandGroup& group);
}


#endif //BASYCO_DEFAULTCOMMANDGROUPPARAMETERS_H
