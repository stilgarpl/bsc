//
// Created by stilgar on 17.11.17.
//

#ifndef BASYCO_STANDARDCOMMANDSDIRECTORY_H
#define BASYCO_STANDARDCOMMANDSDIRECTORY_H


#include "ICommandsDirectory.h"
#include <string>
#include <p2p/network/node/modules/CommandModule.h>


//@todo return type relevant?
template<typename ModuleType, typename ... Args>
struct StandardCommandPack {
    std::string moduleName;
    std::string commandName;

    void (ModuleType::*function)(Args...);
};


template<typename ModuleType, typename ... Args>
class StandardCommandsDirectory : public ICommandsDirectory {
private:
    std::vector<StandardCommandPack<ModuleType, Args...>> data;
public:
    StandardCommandsDirectory(std::initializer_list<StandardCommandPack<ModuleType, Args...>> init) : data(init.begin(),
                                                                                                           init.end()) {

    }

    void configureCommands(CommandModule &commandModule) override {
        for (auto &&datum : data) {
            commandModule.mapCommand(datum.moduleName, datum.commandName, datum.function);
        }
    }

    StandardCommandsDirectory() {};
};


struct StandardCommandsHelper {

    template<typename ModuleType, typename ... Args>
    static StandardCommandsDirectory<ModuleType, Args...>
    init(std::initializer_list<StandardCommandPack<ModuleType, Args...>> init) {
        return StandardCommandsDirectory<ModuleType, Args...>(init);
    }
};

#endif //BASYCO_STANDARDCOMMANDSDIRECTORY_H
