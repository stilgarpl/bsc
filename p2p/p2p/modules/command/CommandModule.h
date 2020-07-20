//
// Created by Krzysztof Tulidowicz on 14.11.17.
//

#ifndef BSC_COMMANDMODULE_H
#define BSC_COMMANDMODULE_H

#include <p2p/core/dependency/DependencyManaged.h>
#include <p2p/core/node/Node.h>
#include <p2p/core/node/context/NodeContext.h>
#include <p2p/core/node/module/NodeModule.h>
#include <p2p/modules/basic/BasicModule.h>
#include <p2p/modules/network/NetworkModule.h>
#include <p2p/modules/network/remote/RemoteNode.h>
#include <parser/cast/templateCast.h>
#include <parser/parameters/CommandLineParameters.h>
#include <parser/parser/explode.h>
#include <utility>

namespace bsc {
    class CommandModule : public NodeModuleDependent<CommandModule, NetworkModule, BasicModule> {
    public:
        using ArgumentContainerType     = std::vector<std::string>;
        using ArgumentContainerTypeCRef = const ArgumentContainerType&;
        enum class CommandExecutionStatus {
            success,
            notEnoughArguments,
            tooManyArguments,
            noCommand,
            badCommand,
        };

    private:
        struct GroupHandlerResult {
            CommandExecutionStatus status;
            ArgumentContainerType arguments;
        };

    public:
        class CommandGroup;

    private:
        using InternalGroupHandlerFunc =
                std::function<GroupHandlerResult(ArgumentContainerTypeCRef, const CommandGroup&)>;

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

            template<typename ModuleType, typename RetType, typename... Args>
            class SpecificCommandData : public CommandData {
            private:
                RetType (ModuleType::*func)(Args... args);

            public:
                SpecificCommandData(const std::string& commandName, RetType (ModuleType::*func)(Args...))
                    : CommandData(commandName), func(func) {}

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
            //            SpecificCommandDataWithParameters(const std::string &commandName, RetType
            //            (ModuleType::*func)(Args...), ParametersType&& params) : CommandData(
            //                    commandName),
            //                                                                                                        func(func),
            //                                                                                                        parameters(std::forward(params))
            //                                                                                                        {}
            //
            //            void applyCommand(CommandModule &commandModule) override {
            //                commandModule.mapCommand(commandName, func);
            //            }
            //            ~SpecificCommandDataWithParameters() override = default;
            //        };

            std::list<std::shared_ptr<CommandData>> commands;

        public:
            template<typename ModuleType, typename RetType, typename... Args>
            void mapCommand(const std::string& commandName, RetType (ModuleType::*f)(Args... args)) {
                auto command = std::make_shared<SpecificCommandData<ModuleType, RetType, Args...>>(commandName, f);
                commands.push_back(command);
                LOGGER("MAPPING COMMAND " + command->getCommandName());
            }

            //        template<typename ModuleType, typename ParametersType, typename RetType, typename ... Args>
            //        void mapCommand(const std::string &commandName, RetType (ModuleType::*f)(Args... args),
            //        ParametersType parameters) {
            //            auto command = std::make_shared<SpecificCommandData<ModuleType, RetType,
            //            Args...>>(commandName, f); commands.push_back(command); LOGGER("MAPPING COMMAND " +
            //            command->getCommandName());
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
            std::map<std::string, std::function<CommandExecutionStatus(ArgumentContainerTypeCRef)>> commands{};
            CommandModule& parent;
            std::map<std::string, std::shared_ptr<CommandGroup>> groups{};
            InternalGroupHandlerFunc groupHandler;
            bool isDefaultHandler = true;

        private:
            auto& getCommandMap() {
                // return commands.get<std::string, std::function<void(ArgumentContainerType)>>();
                return commands;
            }

            const auto& getCommandMap() const {
                // return commands.get<std::string, std::function<void(ArgumentContainerType)>>();
                return commands;
            }

            template<ParametersClass ParametersType>
            void setDefaultGroupHandler(const InternalGroupHandlerFunc& newGroupHandler) {
                if (!groupHandler || isDefaultHandler) {
                    groupHandler     = newGroupHandler;
                    isDefaultHandler = true;
                }
                std::for_each(groups.begin(), groups.end(), [&newGroupHandler](auto& i) {
                    i.second->template setDefaultGroupHandler<ParametersType>(newGroupHandler);
                });
            }

        public:
            explicit CommandGroup(CommandModule& parent) : parent(parent) {}

            CommandGroup& group(const std::string& name);

            template<ParametersClass ParametersType>
            void handler(std::function<CommandExecutionStatus(const ParametersType&

                                                              )> handlerFunc) {
                groupHandler = [handlerFunc](ArgumentContainerTypeCRef arguments,
                                             const CommandModule::CommandGroup& group) -> GroupHandlerResult {
                    auto parameters =
                            CommandLineParser::defaultParse<ParametersType>(arguments, ParseConfiguration::silent);
                    //@todo handlerFunc takes only one parameter... fix it.
                    auto status = handlerFunc(parameters, group);
                    return {status, parameters.arguments()};
                };
                isDefaultHandler = false;
            }

        protected:
            /// template <typename ReturnType, typename ... Args>
            void mapCommand(const std::string& prefix,
                            const std::string& commandName,
                            const std::function<CommandExecutionStatus(ArgumentContainerTypeCRef)>& func) {
                std::string key = prefix + ":::" + commandName;
                auto& map = getCommandMap();// commands.get<std::string, std::function<void(ArgumentContainerType)>>();
                map[key]  = func;
            }

        public:
            template<typename ModuleType, ParametersClass ParametersType, typename RetType, typename... Args>
            void mapCommand(std::string commandName,
                            RetType (ModuleType::*f)(const ParametersType&, Args... args),
                            ParametersType params) {
                parent.addRequiredDependency<ModuleType>();
                auto mod     = parent.node.getModule<ModuleType>();
                auto command = parent.node.getModule<CommandModule>();
                /// long cast is sad :( to_string should have size_t overload
                /// std::to_string((long) sizeof...(Args))
                //@todo prefix or sth, problem is that size of args breaks raw functions that are random
                mapCommand(" ",
                           commandName,
                           [f, mod, commandName, params](CommandModule::ArgumentContainerTypeCRef vals) {
                               ParametersType localParams =
                                       CommandLineParser::defaultParse<ParametersType>(commandName, vals);
                               std::function<RetType(Args...)> func = [mod, localParams, f](Args... args) -> RetType {
                                   ((mod.get())->*f)(localParams, args...);
                               };
                               try {
                                   runStandardFunction(func, localParams.arguments());
                               } catch (const IncorrectParametersException& e) {
                                   if (e.requiredParameters > e.gotParameters) {
                                       return CommandExecutionStatus::notEnoughArguments;
                                   } else {
                                       return CommandExecutionStatus::tooManyArguments;
                                   }
                               }
                               return CommandExecutionStatus::success;
                           });
            }

            template<typename ModuleType, typename RetType, typename... Args>
            void mapCommand(std::string commandName, RetType (ModuleType::*f)(Args... args)) {
                parent.addRequiredDependency<ModuleType>();
                auto mod     = parent.node.getModule<ModuleType>();
                auto command = parent.node.getModule<CommandModule>();
                /// long cast is sad :( to_string should have size_t overload
                /// std::to_string((long) sizeof...(Args))
                //@todo prefix or sth, problem is that size of args breaks raw functions that are random
                mapCommand(" ", commandName, [=](CommandModule::ArgumentContainerTypeCRef vals) {
                    try {
                        runMemberFunction(*mod, f, vals);
                    } catch (const IncorrectParametersException& e) {
                        if (e.requiredParameters > e.gotParameters) {
                            return CommandExecutionStatus::notEnoughArguments;
                        } else {
                            return CommandExecutionStatus::tooManyArguments;
                        }
                    }
                    return CommandExecutionStatus::success;
                });
            }

            template<typename ModuleType, typename RetType>
            void mapRawCommand(std::string commandName, RetType (ModuleType::*f)(ArgumentContainerTypeCRef)) {
                parent.addRequiredDependency<ModuleType>();
                auto mod = parent.node.getModule<ModuleType>();
                mapCommand(" ", commandName, [=](CommandModule::ArgumentContainerTypeCRef vals) {
                    (mod.get()->*f)(vals);
                    return CommandExecutionStatus::success;
                });
            }

            CommandExecutionStatus runCommand(const std::string& commandName, ArgumentContainerTypeCRef arguments) {
                if (groups.contains(commandName)) {
                    auto& thisGroup = groups[commandName];
                    //@todo optimize this, arguments are copied and copied...
                    auto handlerResult = thisGroup->groupHandler
                                                 ? thisGroup->groupHandler(arguments, *thisGroup)
                                                 : GroupHandlerResult{CommandExecutionStatus::success, arguments};
                    if (handlerResult.status == CommandExecutionStatus::success) {
                        ArgumentContainerTypeCRef realArguments = handlerResult.arguments;
                        if (!realArguments.empty()) {
                            // command is a submodule, redirecting
                            //@todo type
                            std::vector<std::string> newArguments(realArguments.begin() + 1, realArguments.end());
                            std::string newCommand = (*realArguments.begin());
                            return group(commandName).runCommand(newCommand, newArguments);
                        } else {
                            // command group invoked without any arguments
                            return CommandExecutionStatus::noCommand;
                        }
                    } else {
                        // handler stopped the execution
                        return handlerResult.status;
                    }
                } else {
                    // command is mapped to this module, execute
                    std::string prefix =
                            " ";//@todo raw pointers and this is colliding std::to_string(arguments.size());
                    LOGGER("Running module " + prefix + " command " + commandName);
                    std::string key = prefix + ":::" + commandName;
                    auto& map       = getCommandMap();
                    if (!map.contains(key)) {
                        LOGGER("FAILURE");
                        return CommandExecutionStatus::badCommand;
                    } else {
                        return map[key](arguments);
                    }
                }
            }

            [[nodiscard]] auto getCommands() const {
                std::list<std::string> retVal;
                auto& map = getCommandMap();
                for ([[maybe_unused]] auto&& [key, value] : map) {
                    retVal.push_back(key);
                }
                return retVal;
            }

            friend class CommandModule;
        };

        friend class CommandGroup;

    private:
        /**
         * map module -> module
         * command -> module->command
         *
         */

        bool interactive = false;

        CommandGroup defaultCommandGroup;
        InternalGroupHandlerFunc defaultGroupHandler;

    public:
        template<ParametersClass ParametersType>
        void setDefaultGroupHandler(
                std::function<CommandExecutionStatus(const ParametersType&, const CommandModule::CommandGroup&)

                              > handlerFunc) {
            //@todo unify duplicated code with CommandGroup
            defaultGroupHandler = [handlerFunc](ArgumentContainerTypeCRef arguments,
                                                const CommandModule::CommandGroup& group) -> GroupHandlerResult {
                auto parameters = CommandLineParser::defaultParse<ParametersType>("", arguments);
                auto status     = handlerFunc(parameters, group);
                return {status, parameters.arguments()};
            };
            defaultCommandGroup.setDefaultGroupHandler<ParametersType>(defaultGroupHandler);
        }

        void prepareSubmodules() override;

        CommandGroup& group(const std::string& name) { return defaultCommandGroup.group(name); }

        CommandGroup& group() { return defaultCommandGroup; }

        explicit CommandModule(INode& node);

    public:
        template<typename ModuleType, typename RetType, typename... Args>
        void mapCommand(std::string commandName, RetType (ModuleType::*f)(Args... args)) {

            group().mapCommand(commandName, f);
        }

        template<typename ModuleType, typename ParametersType, typename RetType, typename... Args>
        void mapCommand(std::string commandName,
                        RetType (ModuleType::*f)(const ParametersType&, Args... args),
                        ParametersType params) {

            group().mapCommand(commandName, f, params);
        }

        template<typename ModuleType, typename RetType>
        void mapRawCommand(std::string commandName, RetType (ModuleType::*f)(ArgumentContainerTypeCRef)) {

            group().mapRawCommand(commandName, f);
        }

        CommandExecutionStatus runCommand(const std::string& commandName, ArgumentContainerTypeCRef arguments = {}) {
            return group().runCommand(commandName, arguments);
            //        }
        }

        void setupActions(ILogicModule::SetupActionHelper& actionHelper) override;

        bool assignActions(ILogicModule::AssignActionHelper& actionHelper) override;

        bool setupSources(ILogicModule::SetupSourceHelper& sourceHelper) override;

    public:
        void runLine(const std::string& line);

        void runInteractive();

        void run() override;

        bool isInteractive() const;

        void setInteractive(bool interactive);

        void initialize() override;

        void sendRemoteCommand(ArgumentContainerTypeCRef args);

        void sendCommandToRemoteNode(RemoteNode& remoteNode, ArgumentContainerTypeCRef args);

        void broadcastRemoteCommand(ArgumentContainerTypeCRef args);

        void runInBackground(ArgumentContainerTypeCRef args);

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

        void testingMethodInt(int a) { LOGGER("Command testing method INT " + std::to_string(a)); }

        void testingMethodIntFloat(int a, float b) {
            LOGGER("Command testing method INT FLOAT " + std::to_string(a) + " " + std::to_string(b));
        }

        struct CommandPP : public CommandLineParameters {
            Parameter<int> a = {{'a', "aaa", "NUM", "IntegerParameter"}};
        };

        void parametersTestingCommand(const CommandPP& params);

        void listCommands() {}

        void sleep(int seconds) { std::this_thread::sleep_for(std::chrono::seconds(seconds)); }
    };
}// namespace bsc

#endif// BSC_COMMANDMODULE_H
