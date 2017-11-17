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

//
//template<std::size_t... Indices>
//struct indices {
//    using next = indices<Indices..., sizeof...(Indices)>;
//};
//template<std::size_t N>
//struct build_indices {
//    using type = typename build_indices<N - 1>::type::next;
//};
//template<>
//struct build_indices<0> {
//    using type = indices<>;
//};
//template<std::size_t N>
//using BuildIndices = typename build_indices<N>::type;
//
//template<typename Iterator>
//using ValueType = typename std::iterator_traits<Iterator>::value_type;

#include <p2p/network/protocol/context/NodeContext.h>
#include <p2p/network/node/modules/command/ICommandsDirectory.h>


class IncorrectParametersException {
public:
    size_t requiredParameters;
    size_t gotParameters;

    IncorrectParametersException(size_t requiredParameters, size_t gotParameters);


};


class CommandModule : public NodeModule, public DependencyManaged<CommandModule> {
private:

    template<typename T, typename RetType, typename ... Args, typename ... Strings>
    static void runStringCast(T &t, RetType (T::*f)(Args...), Strings ... strings) {
        (t.*f)(from_string<Args>(strings)...);
    };


    template<typename RetType, typename ... Args, typename ... Strings>
    static void runStringCast(RetType (*f)(Args...), Strings ... strings) {
        (*f)(from_string<Args>(strings)...);
    };


    template<size_t num_args>
    struct unpack_caller {

    private:
        template<typename T, typename RetType, typename ... Args, size_t... I>
        void call(T &t, RetType(T::*f)(Args...), std::vector<std::string> &args, std::index_sequence<I...>) {
            runStringCast(t, f, args[I]...);
        }

        template<typename RetType, typename ... Args, size_t... I>
        void call(RetType(*f)(Args...), std::vector<std::string> &args, std::index_sequence<I...>) {
            (*f)(args[I]...);
        }


    public:
        template<typename T, typename RetType, typename ... Args>
        void operator()(T &t, RetType(T::*f)(Args...), std::vector<std::string> &args) {
            //assert(args.size() == num_args); // just to be sure
            if (args.size() != num_args) {
                throw IncorrectParametersException(num_args, args.size());
            }
            call(t, f, args, std::make_index_sequence<num_args>{});
        }

        template<typename RetType, typename ... Args>
        void operator()(RetType(*f)(Args...), std::vector<std::string> &args) {
            //assert(args.size() == num_args); // just to be sure
            call(f, args, std::make_index_sequence<num_args>{});
        }
    };


    template<typename T, typename RetType, typename ... Args>
    void runMemberFunction(T &t, RetType (T::*f)(Args...), std::vector<std::string> values) {
        unpack_caller<sizeof... (Args)> a;
        a(t, f, values);
    };





    /**
     * map module -> module
     * command -> module->command
     *
     */

    //this does not have to be a Uber, I'm just using map<string, function>
    Uber<std::map> commands;
    bool interactive = false;
    std::list<std::shared_ptr<ICommandsDirectory>> commandsDirectory;
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
public:

    template<typename ModuleType, typename RetType, typename ... Args>
    void mapCommand(std::string moduleName, std::string commandName, RetType (ModuleType::*f)(Args...)) {
        addRequiredDependency<ModuleType>();
        auto mod = node.getModule<ModuleType>();
        auto command = node.getModule<CommandModule>();
        command->mapCommand(moduleName, commandName, [=](CommandModule::ArgumentContainerType vals) {
            runMemberFunction(*mod, f, vals);
        });
    };

    bool runCommand(std::string moduleName, std::string commandName, ArgumentContainerType arguments) {
        LOGGER("Running module " + moduleName + " command " + commandName);
        std::string key = moduleName + ":::" + commandName;
        auto &map = commands.get<std::string, std::function<void(ArgumentContainerType)>>();
        if (map.count(key) == 0) {
            NODECONTEXTLOGGER("FAILURE");
            return false;
        } else {
            map[key](arguments);
            NODECONTEXTLOGGER("SUCCESS")
            return true;
        }
    }

    void setupActions(LogicManager &logicManager) override;

    bool assignActions(LogicManager &logicManager) override;

    bool setupSources(LogicManager &logicManager) override;

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


    void testingMethod(Dupa a) {
        LOGGER("Command testing method " + std::to_string(a.getA()));
    }

    void testingMethodInt(int a) {
        LOGGER("Command testing method INT " + std::to_string(a));
    }

    void testingMethodIntFloat(int a, float b) {
        LOGGER("Command testing method INT FLOAT " + std::to_string(a) + " " + std::to_string(b));
    }

    void runInteractive() {
        std::string line;
        node.setNodeContextActive();
        while (!isStopping() && std::getline(std::cin, line)) {

            LOGGER("Command: " + line);
            //explode command into words
            auto words = explode(line, ' ');

            std::string module = "";
            std::string commandName = "";
            std::vector<std::string> data;
            if (words.size() > 0) module = words[0];
            if (words.size() > 1) commandName = words[1];
            if (words.size() > 2) {
                auto b = words.begin() + 2;
                auto e = words.end();
                data.insert(data.end(), b, e);
            }

            try {
                runCommand(module, commandName, data);
            } catch (const IncorrectParametersException &e) {
                LOGGER("Incorrect parameters. Required: " + std::to_string(e.requiredParameters) + " got: " +
                       std::to_string(e.gotParameters));
            }

        }
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
};


#endif //BASYCO_COMMANDMODULE_H
