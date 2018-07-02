//
// Created by stilgar on 14.11.17.
//

#ifndef BASYCO_COMMANDMODULE_H
#define BASYCO_COMMANDMODULE_H


#include <p2p/dependency/DependencyManaged.h>
#include <p2p/network/node/NodeModule.h>
#include <p2p/network/node/Node.h>
#include <p2p/utils/from_string.h>

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

#include <p2p/network/protocol/context/NodeContext.h>
#include <p2p/network/node/modules/command/ICommandsDirectory.h>
#include <p2p/utils/template_cast.h>


class CommandModule : public NodeModuleDependent<CommandModule> {
public:
    typedef const std::vector<std::string> &ArgumentContainerType;
public:

    class CommandSubModule {
    private:
        Uber<std::map> commands;
        CommandModule &parent;
        std::map<std::string, std::shared_ptr<CommandSubModule>> submodules;

    private:
        auto &getCommandMap() {
            return commands.get<std::string, std::function<void(ArgumentContainerType)>>();
        }

    public:
        explicit CommandSubModule(CommandModule &parent) : parent(parent) {}

        CommandSubModule &submodule(std::string name);

    protected:
        ///template <typename ReturnType, typename ... Args>
        bool mapCommand(std::string prefix, std::string commandName, std::function<void(ArgumentContainerType)> func) {
            std::string key = prefix + ":::" + commandName;
            auto &map = getCommandMap();//commands.get<std::string, std::function<void(ArgumentContainerType)>>();
            map[key] = func;
            return true;
        };
    public:

        template<typename ModuleType, typename RetType, typename ... Args>
        void mapCommand(std::string commandName, RetType (ModuleType::*f)(Args... args)) {
            parent.addRequiredDependency<ModuleType>();
            auto mod = parent.node.getModule<ModuleType>();
            auto command = parent.node.getModule<CommandModule>();
            ///long cast is sad :( to_string should have size_t overload
            ///std::to_string((long) sizeof...(Args))
            ///@todo prefix or sth, problem is that size of args breaks raw functions that are random
            mapCommand(" ", commandName,
                       [=](CommandModule::ArgumentContainerType vals) {
                           runMemberFunction(*mod, f, vals);
                       });
        };


        template<typename ModuleType, typename RetType>
        void mapRawCommand(std::string commandName, RetType (ModuleType::*f)(ArgumentContainerType)) {
            parent.addRequiredDependency<ModuleType>();
            auto mod = parent.node.getModule<ModuleType>();
            mapCommand(" ", commandName, [=](CommandModule::ArgumentContainerType vals) {
                (mod.get()->*f)(vals);
            });
        };

        bool runCommand(std::string commandName, ArgumentContainerType arguments) {
            ///@todo integrate duplicate code with CommandModule somehow
            if (submodules.count(commandName) > 0 && arguments.size() > 1) {
                //command is a submodule, redirecting
                ///@todo type
                std::vector<std::string> newArguments(arguments.begin() + 1, arguments.end());
                std::string newCommand = (*arguments.begin());
                return submodule(commandName).runCommand(newCommand, newArguments);
                // return false;
            } else {
                //command is mapped to this module, execute
                std::string prefix = " "; ///@todo raw pointers and this is colliding std::to_string(arguments.size());
                LOGGER("Running module " + prefix + " command " + commandName);
                std::string key = prefix + ":::" + commandName;
                auto &map = getCommandMap();
                if (map.count(key) == 0) {
                    NODECONTEXTLOGGER("FAILURE");
                    return false;
                } else {
                    map[key](arguments);
                    NODECONTEXTLOGGER("SUCCESS")
                    return true;
                }
            }
        }

        auto getCommands() {
            std::list<std::string> retVal;
            auto &map = getCommandMap();
            for ([[maybe_unused]]auto &&[key, value] : map) {
                retVal.push_back(key);
            }
        }


    };

    friend class CommandSubModule;

private:


    /**
     * map module -> module
     * command -> module->command
     *
     */

    //this does not have to be a Uber, I'm just using map<string, function>
    Uber<std::map> commands;
    bool interactive = false;
    std::list<std::shared_ptr<ICommandsDirectory>> commandsDirectory;

    std::map<std::string, std::unique_ptr<CommandSubModule>> submodules;
    CommandSubModule defaultSubModule;

public:


    CommandSubModule &submodule(std::string name) {
        if (submodules.count(name) == 0) {
            submodules[name] = std::make_unique<CommandSubModule>(*this);
        }

        return *submodules[name];
    }

    CommandSubModule &submodule() {
        return defaultSubModule;
    }



    CommandModule(INode &node);

public:

    template<typename ModuleType, typename RetType, typename ... Args>
    void mapCommand(std::string commandName, RetType (ModuleType::*f)(Args... args)) {

        ///@todo check if commandName is a module name
        ///@todo also check for collsions in creating submodules
        if (submodules.count(commandName) > 0) {
            ///@todo exception, return false or sth.
            LOGGER("You are trying to add a command that has exactly same name as existing submodule. It won't work.")
        }

        submodule().mapCommand(commandName, f);

    };

    template<typename ModuleType, typename RetType>
    void mapRawCommand(std::string commandName, RetType (ModuleType::*f)(ArgumentContainerType)) {

        ///@todo check if commandName is a module name
        ///@todo also check for collsions in creating submodules
        if (submodules.count(commandName) > 0) {
            ///@todo exception, return false or sth.
            LOGGER("You are trying to add a command that has exactly same name as existing submodule. It won't work.")
        }

        submodule().mapRawCommand(commandName, f);

    };

    bool runCommand(std::string commandName, ArgumentContainerType arguments) {

        if (submodules.count(commandName) > 0 && arguments.size() > 1) {
            ///@todo type
            std::vector<std::string> newArguments(arguments.begin() + 1, arguments.end());
            std::string newCommand = (*arguments.begin());
            return submodule(commandName).runCommand(newCommand, newArguments);
        } else {

            return submodule().runCommand(commandName, arguments);
        }
    }

    void setupActions(ILogicModule::SetupActionHelper &actionHelper) override;

    bool assignActions(ILogicModule::AssignActionHelper &actionHelper) override;

    bool setupSources(ILogicModule::SetupSourceHelper &sourceHelper) override;

private:
    std::vector<std::string> explode(std::string const &s, char delim) {
        std::vector<std::string> result;
        std::istringstream iss(s);

        for (std::string token; std::getline(iss, token, delim);) {
            result.push_back(std::move(token));
        }

        return result;
    }


public:




    void runInteractive() {
        std::string line;
        node.setNodeContextActive();
        while (!isStopping() && std::getline(std::cin, line)) {

            LOGGER("Command: " + line);
            //explode command into words
            auto words = explode(line, ' ');

            std::string module = "";
            //  std::string commandName = "";
            std::vector<std::string> data;
            if (words.size() > 0) module = words[0];
            //if (words.size() > 1) commandName = words[1];
            if (words.size() > 1) {
                auto b = words.begin() + 1;
                auto e = words.end();
                data.insert(data.end(), b, e);
            }

            try {
                runCommand(module, data);
            } catch (const IncorrectParametersException &e) {
                LOGGER("Incorrect parameters. Required: " + std::to_string(e.requiredParameters) + " got: " +
                       std::to_string(e.gotParameters));
            }

        }

        LOGGER("Command Module has stopped")
    }

    void run() {
        ///@todo restart this if flag changes later?
        if (interactive) {
            runInteractive();
        }
    }

    bool isInteractive() const;

    void setInteractive(bool interactive);

    void addCommandsDirectory(std::shared_ptr<ICommandsDirectory> c) {
        commandsDirectory.push_back(c);
    }

    void initialize() override;



    ////////////////////////////////
    /// Commands section
    ////////////////////////////////

    void testingMethod(Dupa a) {
        LOGGER("Command testing method " + std::to_string(a.getA()));
    }

    void testingMethodInt(int a) {
        LOGGER("Command testing method INT " + std::to_string(a));
    }

    void testingMethodIntFloat(int a, float b) {
        LOGGER("Command testing method INT FLOAT " + std::to_string(a) + " " + std::to_string(b));
    }


    void sendRemoteCommand(ArgumentContainerType args);

    void listCommands() {

    }

};


#endif //BASYCO_COMMANDMODULE_H
