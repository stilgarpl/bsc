//
// Created by stilgar on 13.01.2020.
//

#ifndef BSC_DEFAULTCOMMANDGROUPPARAMETERS_H
#define BSC_DEFAULTCOMMANDGROUPPARAMETERS_H


#include <parser/parameters/CommandLineParameters.h>
#include "CommandModule.h"


namespace bsc {
    struct DefaultCommandGroupParameters : bsc::CommandLineParameters {
        Flag help = {{.shortKey = 'h', .longKey = "help", .doc = "Shows help", .defaultValue = false}};
    };

    CommandModule::CommandExecutionStatus
    defaultCommandGroupHandler(const bsc::DefaultCommandGroupParameters& params,
                               const CommandModule::CommandGroup& group);
}


#endif //BSC_DEFAULTCOMMANDGROUPPARAMETERS_H
