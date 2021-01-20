//
// Created by Krzysztof Tulidowicz on 07.01.2020.
//

#ifndef BSC_COMMANDLINEPARAMETERS_H
#define BSC_COMMANDLINEPARAMETERS_H

#include <argp.h>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <parser/parser/fromString.h>
#include <set>
#include <span>
#include <utility>

namespace bsc {

    enum class ParseConfiguration {
        simple,
        silent,
    };

    class CommandLineParameters;

    template<typename T>
    concept ParametersClass = std::is_base_of_v<CommandLineParameters, T>;

    class ValueNotAllowed : public std::domain_error {
    public:
        const std::set<std::string> allowedValues;

        explicit ValueNotAllowed(const std::string& arg);
        ValueNotAllowed(const std::string& arg, std::remove_cvref_t<decltype(allowedValues)> a);
        ValueNotAllowed(const ValueNotAllowed&) = default;
        ValueNotAllowed(ValueNotAllowed&&)      = default;
    };

    class CommandLineParameters {
    private:
        class ParserBuilder;

        class ArgumentParser {
        public:
            using OptionParseFunc   = std::function<void(const char*, Parser&)>;
            using ArgumentParseFunc = std::function<void(const std::string&, Parser&)>;

        private:
            std::vector<argp_option> argpOptions{};
            std::string doc{};
            std::string argDoc{};
            argp argParams{};
            ParseConfiguration parseConfiguration;
            unsigned flags{};
            std::map<decltype(argp_option::key), OptionParseFunc> parseMap{};
            std::vector<std::string> rawArguments{};
            std::string commandName;
            struct ArgumentDescriptor {
                ArgumentParseFunc argumentParseFunc{};
                decltype(rawArguments)::size_type argumentIndex{};
                std::optional<std::string> argumentName{};
            };
            std::vector<ArgumentDescriptor> argumentDescriptors{};
            std::vector<std::string> usageDocs    = {};
            std::optional<std::string> beforeInfo = std::nullopt;
            std::optional<std::string> afterInfo                     = std::nullopt;
            decltype(rawArguments)::size_type requiredArgumentsCount = 0;
            Parser stringParser{};

            void incrementRequiredArguments() { ++requiredArgumentsCount; }
            void parseNamedArguments();

        public:
            static error_t parseArgument(int key, char* arg, struct argp_state* state);

            void prepareParser(ParseConfiguration configuration, const Parser&);
            void parse(int argc, char* argv[]);
            static char* helpFilter(int key, const char* text, void* input);
            auto& getParsedArguments() { return rawArguments; }
            auto getRemainingArguments() {
                return std::span<std::string>(rawArguments.begin() + requiredArgumentsCount, rawArguments.end());
            };
            auto& getCommandName() { return commandName; }

            auto getRequiredArgumentsCount() const { return requiredArgumentsCount; }

            friend class CommandLineParameters::ParserBuilder;
            friend class CommandLineParser;
            void prepareArgumentUsage();
        };

        class ParserBuilder {
        private:
            std::shared_ptr<ArgumentParser> parser = nullptr;
            int currentKey                         = 0;

        public:
            struct ParserOptions {
                std::optional<char> shortKey{};
                std::optional<std::string_view> longKey{};
                std::optional<std::string_view> argumentName{};
                int flags{};
                std::optional<std::string_view> doc{};
            };
            void addOption(ParserOptions parserOptions, ArgumentParser::OptionParseFunc parserFunction);

            void addGroup(const char* doc);
            void addAlias(char shortKey, const char* longKey = nullptr);
            void addAlias(const char* longKey);
            void addUsage(std::string usage);

            void addDoc(std::string doc);
            void addArgument(ArgumentParser::ArgumentParseFunc parserFunction,
                             std::optional<std::string> argumentName = std::nullopt);

            std::shared_ptr<ArgumentParser> make();

            void reset();
        };

        static ParserBuilder& parserBuilder() {
            static thread_local ParserBuilder parserBuilder;
            return parserBuilder;
        }

        const std::shared_ptr<ArgumentParser> parser;

        template<typename T>
        friend class BaseParameter;

        template<typename T>
        friend class Parameter;

        template<typename T>
        friend class OptionalParameter;

        template<typename T>
        friend class RequiredParameter;

        friend class Group;
        template<typename T>
        friend class Argument;
        friend class Usage;

        friend class Doc;

        friend class Alias;

    public:
        CommandLineParameters();

        friend class CommandLineParser;

        [[nodiscard]] const std::vector<std::string>& arguments() const { return parser->getParsedArguments(); }
        [[nodiscard]] std::span<std::string> remainingArguments() const { return parser->getRemainingArguments(); }
        [[nodiscard]] auto getRequiredArgumentsCount() const { return parser->getRequiredArgumentsCount(); }

        [[nodiscard]] const std::string& commandName() const { return parser->getCommandName(); }
    };

    class CommandLineParser {
    private:
        ParseConfiguration parseConfiguration = ParseConfiguration::simple;
        const Parser parser{};

    public:
        CommandLineParser(ParseConfiguration parseConfiguration, const Parser& parser)
            : parseConfiguration(parseConfiguration), parser(parser) {}
        CommandLineParser() = default;

        template<ParametersClass T>
        [[nodiscard]] T parse(int argc, char* argv[]) {
            T t;
            t.parser->prepareParser(parseConfiguration, parser);
            t.parser->parse(argc, argv);
            return t;
        }

        template<ParametersClass T>
        [[nodiscard]] T parse(const std::vector<std::string>& vals) {
            // guarantee contiguous, null terminated strings
            std::vector<std::vector<char>> vstrings;
            // pointers to those strings
            std::vector<char*> cstrings;
            vstrings.reserve(vals.size() + 1);
            cstrings.reserve(vals.size() + 1);

            for (const auto& val : vals) {
                vstrings.emplace_back(val.begin(), val.end());
                vstrings.back().push_back('\0');
                cstrings.push_back(vstrings.back().data());
            }
            return this->parse<T>(cstrings.size(), cstrings.data());
        }

        template<ParametersClass T>
        [[nodiscard]] T parse(const std::string& commandName, std::vector<std::string> vals) {
            vals.insert(vals.begin(), commandName);
            return this->parse<T>(vals);
        }

        template<ParametersClass T>
        [[nodiscard]] static T defaultParse(int argc, char* argv[]) {
            static CommandLineParser commandLineParser;
            return commandLineParser.parse<T>(argc, argv);
        }

        template<ParametersClass T>
        [[nodiscard]] static T defaultParse(const std::vector<std::string>& vals) {
            static CommandLineParser commandLineParser;
            return commandLineParser.parse<T>(vals);
        }

        template<ParametersClass T>
        [[nodiscard]] static T defaultParse(const std::string& commandName, std::vector<std::string> vals) {
            static CommandLineParser commandLineParser;
            return commandLineParser.parse<T>(commandName, vals);
        }
    };

    template<typename T>
    class BaseParameter {
    public:
        class AllowedValues {
        public:
            using AllowedValuesSet = std::set<std::string>;
            using GetterFunc       = std::function<AllowedValuesSet(void)>;

        private:
            GetterFunc getter = []() { return AllowedValuesSet{}; };

        public:
            AllowedValues(std::initializer_list<AllowedValuesSet ::value_type> list) {
                std::set<T> set = list;
                getter          = [set]() { return set; };
            }
            AllowedValues(AllowedValuesSet set) {
                getter = [set]() { return set; };
            }
            template<typename Func>
            AllowedValues(Func func) requires std::is_invocable_r_v<AllowedValuesSet, Func> {
                getter = func;
            }

            AllowedValues()                     = default;
            AllowedValues(const AllowedValues&) = default;
            AllowedValues(AllowedValues&&)      = default;
            AllowedValuesSet get() { return getter(); }
        };

    protected:
        std::optional<T> value;

    private:
        int counter = 0;
        AllowedValues allowedValues{};

        CommandLineParameters::ArgumentParser::OptionParseFunc makeParseFunction() {
            return [this](const char* input, Parser& parser) {
                std::string text = input != nullptr ? input : "";
                //@todo maybe this should be optimized so it is only called once
                const auto& validValues = this->allowedValues.get();
                //@todo case sensitive or not
                if (!validValues.empty() && !validValues.contains(text)) {
                    using namespace std::string_literals;
                    throw ValueNotAllowed("Value "s + text + " is not allowed.", validValues);
                }
                if (!value) {
                    value = parser.fromString<T>(text);
                } else {
                    // if parameter is mentioned multiple times and it's a container, combine options. otherwise,
                    // overwrite.
                    if constexpr (IsContainerNotString<T>) {
                        auto tempValue = parser.fromString<T>(text);
                        std::for_each(tempValue.begin(), tempValue.end(), [this](auto& i) {
                            value->insert(value->end(), i);
                        });
                    } else {
                        value = parser.fromString<T>(text);
                    }
                }

                counter++;
            };
        }

        int makeFlags(bool optional, bool hidden) {
            int flags = 0;
            if (optional) flags |= OPTION_ARG_OPTIONAL;
            if (hidden) flags |= OPTION_HIDDEN;
            return flags;
        }

    protected:
        void setValue(const T& v) { value = v; }

    public:
        //@todo maybe I should add callback here that will be called after this value is set?
        struct BaseParameterDefinition {
            std::optional<char> shortKey{};
            std::optional<std::string_view> longKey{};
            std::optional<std::string_view> argumentName{};
            std::optional<std::string_view> doc{};
            bool optional = false;
            bool hidden   = false;
            std::optional<T> defaultValue{};
            AllowedValues allowedValues{};
        };

        BaseParameter(BaseParameterDefinition def) : allowedValues(def.allowedValues) {
            value         = def.defaultValue;
            auto& builder = bsc::CommandLineParameters::parserBuilder();
            builder.addOption({.shortKey     = def.shortKey,
                               .longKey      = def.longKey,
                               .argumentName = def.argumentName,
                               .flags        = makeFlags(def.optional, def.hidden),
                               .doc          = def.doc},
                              makeParseFunction());
        }

        const decltype(value)& operator()() const { return value; }

        [[nodiscard]] auto count() const { return counter; }
    };

    template<typename T>
    class Parameter : public BaseParameter<T> {

    public:
        using AllowedValues = typename BaseParameter<T>::AllowedValues;
        struct ParameterDefinition {
            std::optional<char> shortKey{};
            std::optional<std::string_view> longKey{};
            std::optional<std::string_view> argumentName{};
            std::optional<std::string_view> doc{};
            std::optional<T> defaultValue{};
            AllowedValues allowedValues{};
        };

        Parameter(ParameterDefinition def)// NOLINT
            : BaseParameter<T>({.shortKey      = def.shortKey,
                                .longKey       = def.longKey,
                                .argumentName  = def.argumentName,
                                .doc           = def.doc,
                                .defaultValue  = def.defaultValue,
                                .allowedValues = def.allowedValues}) {}
    };

    template<typename T>
    class DefaultParameter : public BaseParameter<T> {

    public:
        using AllowedValues = typename BaseParameter<T>::AllowedValues;
        struct DefaultParameterDefinition {
            std::optional<char> shortKey{};
            std::optional<std::string_view> longKey{};
            std::optional<std::string_view> argumentName{};
            std::optional<std::string_view> doc{};
            T defaultValue{};
            AllowedValues allowedValues{};
        };

        DefaultParameter(DefaultParameterDefinition def)// NOLINT
            : BaseParameter<T>({.shortKey      = def.shortKey,
                                .longKey       = def.longKey,
                                .argumentName  = def.argumentName,
                                .doc           = def.doc,
                                .defaultValue  = std::move(def.defaultValue),
                                .allowedValues = def.allowedValues}) {}

        const auto& operator()() const { return *this->value; }// NOLINT
    };

    template<typename T>
    class OptionalParameter : public BaseParameter<T> {
    public:
        OptionalParameter(char shortKey,
                          const char* longKey,
                          const char* argumentName,
                          const char* doc,
                          const std::optional<T>& defaultValue)
            : BaseParameter<T>(shortKey, longKey, doc, argumentName, defaultValue, true, false) {}

        OptionalParameter(char shortKey, const char* argumentName, const char* doc)
            : BaseParameter<T>(shortKey, argumentName, doc, true, false) {}

        OptionalParameter(const char* longKey, const char* argumentName, const char* doc)
            : BaseParameter<T>(longKey, argumentName, doc, true, false) {}
    };

    template<typename T>
    class HiddenParameter : public OptionalParameter<T> {};

    class Group {

    public:
        Group(const char* doc) {
            auto& builder = CommandLineParameters::parserBuilder();
            builder.addGroup(doc);
        }
    };

    class Alias {
    public:
        Alias(char shortKey) {
            auto& builder = CommandLineParameters::parserBuilder();
            builder.addAlias(shortKey);
        }

        Alias(const char* longKey) {
            auto& builder = CommandLineParameters::parserBuilder();
            builder.addAlias(longKey);
        }

        Alias(char key, const char* longKey) {
            auto& builder = CommandLineParameters::parserBuilder();
            builder.addAlias(key, longKey);
        }
    };

    class Usage {
    public:
        Usage(std::string usage) {
            auto& builder = CommandLineParameters::parserBuilder();
            builder.addUsage(std::move(usage));
        }

        Usage(const std::vector<std::string>& usage) {
            auto& builder = CommandLineParameters::parserBuilder();
            for (const auto& item : usage) {
                builder.addUsage(item);
            }
        }
    };

    class Doc {
    public:
        Doc(std::string doc) {
            auto& builder = CommandLineParameters::parserBuilder();
            builder.addDoc(std::move(doc));
        }
    };

    template<typename T>
    class RequiredParameter : public BaseParameter<T> {
    public:
        RequiredParameter(char shortKey, const char* longKey, const char* argumentName, const char* doc)
            : BaseParameter<T>(shortKey, longKey, argumentName, doc, false, false) {}

        RequiredParameter(char shortKey, const char* argumentName, const char* doc)
            : BaseParameter<T>(shortKey, argumentName, doc, false, false) {}

        RequiredParameter(const char* longKey, const char* argumentName, const char* doc)
            : BaseParameter<T>(longKey, argumentName, doc, false, false) {}
    };

    using Flag = Parameter<bool>;

    /**
     * Named argument from command line
     */
    template<typename T>
    class Argument {
    private:
        std::optional<T> value;
        CommandLineParameters::ArgumentParser::ArgumentParseFunc makeParseFunction() {
            return [this](const std::string& text, Parser& parser) {
                // this if is probably not necessary, it will be a bug to call it more than once.
                if (!value) {
                    value = parser.fromString<T>(text);
                }
            };
        }

    public:
        Argument() {
            auto& builder = CommandLineParameters::parserBuilder();
            builder.addArgument(makeParseFunction());
        }

        Argument(const std::string& name) {
            auto& builder = CommandLineParameters::parserBuilder();
            builder.addArgument(makeParseFunction(), name);
        }

        const auto& operator()() const { return *this->value; }
    };
}// namespace bsc

#endif// BSC_COMMANDLINEPARAMETERS_H
