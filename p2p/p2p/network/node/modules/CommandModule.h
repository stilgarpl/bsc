//
// Created by stilgar on 14.11.17.
//

#ifndef BASYCO_COMMANDMODULE_H
#define BASYCO_COMMANDMODULE_H


#include <p2p/dependency/DependencyManaged.h>
#include <p2p/network/node/NodeModule.h>
#include <p2p/network/node/Node.h>

///@todo remove class
class Dupa {
private:
    int a;
public:
    Dupa(const std::string &s) {
        a = std::atoi(s.c_str());
    }

    int getA() const {
        return a;
    }
};




class CommandModule : public NodeModule, public DependencyManaged<CommandModule> {

    /**
     * map module -> module
     * command -> module->command
     *
     */

    Uber<std::map> commands;

public:


    typedef const std::vector<std::string> &ArgumentContainerType;

    CommandModule(INode &node);

    ///template <typename ReturnType, typename ... Args>
    bool mapCommand(std::string moduleName, std::string commandName, std::function<void(ArgumentContainerType)> func) {
        std::string key = moduleName + ":::" + commandName;
        auto &map = commands.get<std::string, std::function<void(ArgumentContainerType)>>();
        map[key] = func;
        return true;
    };

//    template<typename ModuleType>
//    bool mapCommandToModule(std::string moduleName, std::string commandName) {
//        Node& nnode = (Node)node;
//        nnode.getModule<ModuleType>()
//    }

    bool runCommand(std::string moduleName, std::string commandName, ArgumentContainerType arguments) {
        LOGGER("Running module " + moduleName + " command " + commandName);
        std::string key = moduleName + ":::" + commandName;
        auto &map = commands.get<std::string, std::function<void(ArgumentContainerType)>>();
        if (map.count(key) == 0) {
            LOGGER("FAILURE");
            return false;
        } else {
            map[key](arguments);
            LOGGER("SUCCESS")
            return true;
        }
    }

    void setupActions(LogicManager &logicManager) override;

    bool assignActions(LogicManager &logicManager) override;

    bool setupSources(LogicManager &logicManager) override;


public:

    void testingMethod(Dupa a) {
        LOGGER("Command testing method " + std::to_string(a.getA()));
    }

    void testingMethodInt(int a) {
        LOGGER("Command testing method INT " + std::to_string(a));
    }

};


#endif //BASYCO_COMMANDMODULE_H
