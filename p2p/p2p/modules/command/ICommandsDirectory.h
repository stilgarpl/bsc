//
// Created by stilgar on 17.11.17.
//

#ifndef BASYCO_ICOMMANDSDIRECTORY_H
#define BASYCO_ICOMMANDSDIRECTORY_H


class CommandModule;

class ICommandsDirectory {
public:

    virtual void configureCommands(CommandModule &commandModule)= 0;

};

#endif //BASYCO_ICOMMANDSDIRECTORY_H
