#include <utility>

//
// Created by stilgar on 14.11.17.
//

#ifndef BASYCO_COMMANDMODULE_H
#define BASYCO_COMMANDMODULE_H


#include <p2p/dependency/DependencyManaged.h>
#include <p2p/node/module/NodeModule.h>
#include <p2p/node/Node.h>
#include <core/utils/from_string.h>
#include <p2p/modules/network/remote/RemoteNode.h>
#include <p2p/modules/network/NetworkModule.h>
#include <p2p/node/context/NodeContext.h>
#include <p2p/modules/command/ICommandsDirectory.h>
#include <core/utils/template_cast.h>
#include <core/parameters/ProgramParameters.h>


class CommandModule : public NodeModuleDependent<CommandModule, NetworkModule> {
public:
    using ArgumentContainerType = std::vector<std::string>;
    typedef const ArgumentContainerType& ArgumentContainerTypeRef;

public:

    class SubModule {
    private:
        class CommandData {
        protected:
            std::string commandName;

        public:
            explicit CommandData(std::string commandName);

            virtual void applyCommand(CommandModule& commandModule) = 0;

            const std::string& getCommandName() const;

            virtual ~CommandData() = default;

        };

        template<typename ModuleType, typename RetType, typename ... Args>
        class SpecificCommandData : public CommandData {
        private:
            RetType (ModuleType::*func)(Args... args);

        public:
            SpecificCommandData(const std::string& commandName, RetType (ModuleType::*func)(Args...)) : CommandData(
                    commandName),
                                                                                                        func(func) {}

            void applyCommand(CommandModule& commandModule) override {
                commandModule.mapCommand(commandName, func);
            }

            ~SpecificCommandData() override = default;
        };

//        template<typename ModuleType, typename RetType, typename ParametersType, typename ... Args>
//        class SpecificCommandDataWithParameters : public CommandData {
//        private:
//            RetType (ModuleType::*func)(Args... args);
//            ParametersType parameters;
//
//        public:
//            SpecificCommandDataWithParameters(const std::string &commandName, RetType (ModuleType::*func)(Args...), ParametersType&& params) : CommandData(
//                    commandName),
//                                                                                                        func(func), parameters(std::forward(params)) {}
//
//            void applyCommand(CommandModule &commandModule) override {
//                commandModule.mapCommand(commandName, func);
//            }
//            ~SpecificCommandDataWithParameters() override = default;
//        };


        std::list<std::shared_ptr<CommandData>> commands;


    public:
        template<typename ModuleType, typename RetType, typename ... Args>
        void mapCommand(const std::string& commandName, RetType (ModuleType::*f)(Args... args)) {
            auto command = std::make_shared<SpecificCommandData<ModuleType, RetType, Args...>>(commandName, f);
            commands.push_back(command);
            LOGGER("MAPPING COMMAND " + command->getCommandName());
        }

//        template<typename ModuleType, typename ParametersType, typename RetType, typename ... Args>
//        void mapCommand(const std::string &commandName, RetType (ModuleType::*f)(Args... args), ParametersType parameters) {
//            auto command = std::make_shared<SpecificCommandData<ModuleType, RetType, Args...>>(commandName, f);
//            commands.push_back(command);
//            LOGGER("MAPPING COMMAND " + command->getCommandName());
//        }

        void applyCommands(CommandModule& commandModule) {
            for (const auto& item : commands) {
                LOGGER("APPLYING COMMAND " + item->getCommandName())
                item->applyCommand(commandModule);
            }
        }
    };

    class CommandGroup {
    private:
//        Uber<std::map> commands;
        std::map<std::string, std::function<void(ArgumentContainerTypeRef)>> commands{};
        CommandModule& parent;
        std::map<std::string, std::shared_ptr<CommandGroup>> groups{};
        std::function<ArgumentContainerType(ArgumentContainerTypeRef)> groupHandler;

    private:
        auto& getCommandMap() {
            //return commands.get<std::string, std::function<void(ArgumentContainerType)>>();
            return commands;
        }

    public:
        explicit CommandGroup(CommandModule& parent) : parent(parent) {}

        CommandGroup& group(std::string name);

        template<ParametersClass ParametersType>
        void handler(std::function<void(const ParametersType&)> handlerFunc) {
            groupHandler = [handlerFunc](ArgumentContainerTypeRef arguments) {
                auto parameters = ProgramParameters::parse<ParametersType>(arguments);
                handlerFunc(parameters); //@todo return value? success? failure? anything?
                return parameters.arguments();

            };
        }

    protected:
        ///template <typename ReturnType, typename ... Args>
        bool mapCommand(const std::string& prefix, const std::string& commandName,
                        const std::function<void(ArgumentContainerTypeRef)>& func) {
            std::string key = prefix + ":::" + commandName;
            auto& map = getCommandMap();//commands.get<std::string, std::function<void(ArgumentContainerType)>>();
            map[key] = func;
            return true;
        }

    public:

        template<typename ModuleType, ParametersClass ParametersType, typename RetType, typename ... Args>
        void mapCommand(std::string commandName, RetType (ModuleType::*f)(const ParametersType&, Args... args),
                        ParametersType params) {
            parent.addRequiredDependency<ModuleType>();
            auto mod = parent.node.getModule<ModuleType>();
            auto command = parent.node.getModule<CommandModule>();
            ///long cast is sad :( to_string should have size_t overload
            ///std::to_string((long) sizeof...(Args))
            //@todo prefix or sth, problem is that size of args breaks raw functions that are random
            mapCommand(" ", commandName,
                       [f, mod, commandName, params](CommandModule::ArgumentContainerTypeRef vals) {

                           ParametersType localParams = ProgramParameters::parse<ParametersType>(commandName, vals);
                           std::function<RetType(Args...)> func = [mod, localParams, f](Args... args) -> RetType {
                               ((mod.get())->*f)(localParams, args...);
                           };
                           runStandardFunction(func, vals);
                       });
        }


        template<typename ModuleType, typename RetType, typename ... Args>
        void mapCommand(std::string commandName, RetType (ModuleType::*f)(Args... args)) {
            parent.addRequiredDependency<ModuleType>();
            auto mod = parent.node.getModule<ModuleType>();
            auto command = parent.node.getModule<CommandModule>();
            ///long cast is sad :( to_string should have size_t overload
            ///std::to_string((long) sizeof...(Args))
            //@todo prefix or sth, problem is that size of args breaks raw functions that are random
            mapCommand(" ", commandName,
                       [=](CommandModule::ArgumentContainerTypeRef vals) {
                           runMemberFunction(*mod, f, vals);
                       });
        }


        template<typename ModuleType, typename RetType>
        void mapRawCommand(std::string commandName, RetType (ModuleType::*f)(ArgumentContainerTypeRef)) {
            parent.addRequiredDependency<ModuleType>();
            auto mod = parent.node.getModule<ModuleType>();
            mapCommand(" ", commandName, [=](CommandModule::ArgumentContainerTypeRef vals) {
                (mod.get()->*f)(vals);
            });
        }

        bool runCommand(const std::string& commandName, ArgumentContainerTypeRef arguments) {
            if (groups.contains(commandName)) {
                auto& thisGroup = groups[commandName];
                ArgumentContainerTypeRef realArguments = thisGroup->groupHandler ? thisGroup->groupHandler(arguments)
                                                                                 : arguments;
                if (realArguments.size() > 1) {
                    //command is a submodule, redirecting
                    //@todo type
                    std::vector<std::string> newArguments(realArguments.begin() + 1, realArguments.end());
                    std::string newCommand = (*realArguments.begin());
                    return group(commandName).runCommand(newCommand, newArguments);
                    // return false;
                } else {
                    //command group invoked without any arguments
                    return false;
                }
            } else {
                //command is mapped to this module, execute
                std::string prefix = " "; //@todo raw pointers and this is colliding std::to_string(arguments.size());
                LOGGER("Running module " + prefix + " command " + commandName);
                std::string key = prefix + ":::" + commandName;
                auto& map = getCommandMap();
                if (map.count(key) == 0) {
                    LOGGER("FAILURE");
                    return false;
                } else {
                    map[key](arguments);
                    LOGGER("SUCCESS")
                    return true;
                }
            }
        }

        auto getCommands() {
            std::list<std::string> retVal;
            auto& map = getCommandMap();
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

    bool interactive = false;
    std::list<std::shared_ptr<ICommandsDirectory>> commandsDirectory;

//    std::map<std::string, std::unique_ptr<CommandGroup>> commandGroups;
    CommandGroup defaultCommandGroup;

public:

    void prepareSubmodules() override;

    CommandGroup& group(const std::string& name) {
//        if (!commandGroups.contains(name)) {
//            commandGroups[name] = std::make_unique<CommandGroup>(*this);
//        }
//
//        return *commandGroups[name];
        return defaultCommandGroup.group(name);
    }

    CommandGroup& group() {
        return defaultCommandGroup;
    }


    explicit CommandModule(INode& node);

public:

    template<typename ModuleType, typename RetType, typename ... Args>
    void mapCommand(std::string commandName, RetType (ModuleType::*f)(Args... args)) {

        //@todo check if commandName is a module name
        //@todo also check for collsions in creating submodules
//        if (commandGroups.contains(commandName)) {
//            //@todo exception, return false or sth.
//            LOGGER("You are trying to add a command that has exactly same name as existing submodule. It won't work.")
//        }

        group().mapCommand(commandName, f);

    }

    template<typename ModuleType, typename ParametersType, typename RetType, typename ... Args>
    void mapCommand(std::string commandName, RetType (ModuleType::*f)(const ParametersType&, Args... args),
                    ParametersType params) {
        //@todo check if commandName is a module name
        //@todo also check for collsions in creating submodules
//        if (commandGroups.contains(commandName)) {
//            //@todo exception, return false or sth.
//            LOGGER("You are trying to add a command that has exactly same name as existing submodule. It won't work.")
//        }

        group().mapCommand(commandName, f, params);
    }

    template<typename ModuleType, typename RetType>
    void mapRawCommand(std::string commandName, RetType (ModuleType::*f)(ArgumentContainerTypeRef)) {

        //@todo check if commandName is a module name
        //@todo also check for collsions in creating submodules
//        if (commandGroups.count(commandName) > 0) {
//            //@todo exception, return false or sth.
//            LOGGER("You are trying to add a command that has exactly same name as existing submodule. It won't work.")
//        }

        group().mapRawCommand(commandName, f);

    }

    bool runCommand(const std::string& commandName, ArgumentContainerTypeRef arguments) {
        return group().runCommand(commandName, arguments);
//        }
    }

    void setupActions(ILogicModule::SetupActionHelper& actionHelper) override;

    bool assignActions(ILogicModule::AssignActionHelper& actionHelper) override;

    bool setupSources(ILogicModule::SetupSourceHelper& sourceHelper) override;

private:
    static std::vector<std::string> explode(std::string const& s, char delim) {
        std::vector<std::string> result;
        std::istringstream iss(s);

        for (std::string token; std::getline(iss, token, delim);) {
            result.push_back(std::move(token));
        }

        return result;
    }


public:


    void runLine(const std::string& line) {
        LOGGER("Command: " + line);
        //explode command into words
        auto words = explode(line, ' ');

        std::string groupOrCommandName = "";
        //  std::string commandName = "";
        std::vector<std::string> data;
        if (!words.empty()) groupOrCommandName = words[0];
        //if (words.size() > 1) commandName = words[1];
        if (words.size() > 1) {
            auto b = words.begin() + 1;
            auto e = words.end();
            data.insert(data.end(), b, e);
        }

        try {
            runCommand(groupOrCommandName, data);
        } catch (const IncorrectParametersException& e) {
            LOGGER("Incorrect parameters. Required: " + std::to_string(e.requiredParameters) + " got: " +
                   std::to_string(e.gotParameters));
        }
    }


    void runInteractive() {
        std::string line;
        node.setNodeContextActive();
        while (!isStopping() && std::getline(std::cin, line)) {
            runLine(line);


        }

        LOGGER("Command Module has stopped")
    }

    void run() override {
        //@todo restart this if flag changes later?
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

    void sendRemoteCommand(ArgumentContainerTypeRef args);

    void sendCommandToRemoteNode(RemoteNode& remoteNode, ArgumentContainerTypeRef args);

    void broadcastRemoteCommand(ArgumentContainerTypeRef args);

    void runInBackground(ArgumentContainerTypeRef args);

    void runScript(const fs::path& scriptPath) {
        if (fs::exists(scriptPath)) {
            std::string line;
            std::ifstream stream(scriptPath);
            while (std::getline(stream, line)) {
                runLine(line);
            }

        } else {
            //@todo error handling
            LOGGER("script does not exist")
        }

    }


    ////////////////////////////////
    /// Commands section
    ////////////////////////////////

    void testingMethodInt(int a) {
        LOGGER("Command testing method INT " + std::to_string(a));
    }

    void testingMethodIntFloat(int a, float b) {
        LOGGER("Command testing method INT FLOAT " + std::to_string(a) + " " + std::to_string(b));
    }

    struct CommandPP : public ProgramParameters {
        Parameter<int> a = {'a', "aaa", "NUM", "IntegerParameter"};
    };

    void parametersTestingCommand(const CommandPP& params);


    void listCommands() {

    }

    void sleep(int seconds) {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }


};


#endif //BASYCO_COMMANDMODULE_H
