//
// Created by stilgar on 07.01.2020.
//

#ifndef BSC_COMMANDLINEPARAMETERS_H
#define BSC_COMMANDLINEPARAMETERS_H


#include <optional>
#include <functional>
#include <argp.h>
#include <iostream>
#include <map>
#include <utility>
#include <memory>
#include "parser/parser/fromString.h"


namespace bsc {

    enum class ParseConfiguration {
        simple,
        silent,
    };


    class CommandLineParameters;

    template<typename T>
    concept ParametersClass = std::is_base_of_v<CommandLineParameters, T>;

    class CommandLineParameters {
    private:
        class ParserBuilder;

        class ArgumentParser {
        public:
            using ParseFunc = std::function<void(const char*)>;

        private:
            std::vector<argp_option> argpOptions;
            std::string doc;
            std::string argDoc;
            argp argParams;
            unsigned flags;
            std::map<decltype(argp_option::key), ParseFunc> parseMap;
            std::vector<std::string> parsedArguments;
            std::vector<std::string> usageDocs = {};
            std::optional<std::string> beforeInfo = std::nullopt;
            std::optional<std::string> afterInfo = std::nullopt;

        public:
            static error_t parseArgument(int key, char* arg, struct argp_state* state);

            void
            prepareParser(ParseConfiguration parseConfiguration);
            void parse(int argc, char* argv[]);
            static char* helpFilter(int key, const char* text, void* input);
            auto& gerParsedArguments() {
                return parsedArguments;
            }

            friend class CommandLineParameters::ParserBuilder;


        };

        class ParserBuilder {
        private:
            std::shared_ptr<ArgumentParser> parser = nullptr;
            int currentKey = 0;

        public:
            void addOption(char shortKey, const char* longKey, const char* argumentName, int flags, const char* doc,
                           ArgumentParser::ParseFunc parserFunction);

            void addOption(char shortKey, const char* argumentName, int flags, const char* doc,
                           ArgumentParser::ParseFunc parserFunction);

            void
            addOption(const char* longKey, const char* argumentName, int flags, const char* doc,
                      ArgumentParser::ParseFunc parserFunction);

            void addGroup(const char* doc);
            void addAlias(char shortKey);
            void addAlias(char shortKey, const char* longKey);
            void addAlias(const char* longKey);
            void addUsage(std::string usage);

            void addDoc(std::string doc);

            std::shared_ptr<ArgumentParser> make();

            void reset();
        };

        static ParserBuilder& parserBuilder() {
            static thread_local ParserBuilder parserBuilder;
            return parserBuilder;
        }


        const std::shared_ptr<ArgumentParser> parser;


        template<typename T>
        friend
        class BaseParameter;

        template<typename T>
        friend
        class Parameter;

        template<typename T>
        friend
        class OptionalParameter;

        template<typename T>
        friend
        class RequiredParameter;

        friend class Group;

        friend class Usage;

        friend class Doc;

        friend class Alias;

    public:
        CommandLineParameters();


        template<ParametersClass T>
        [[nodiscard]] static T
        parse(int argc, char* argv[], ParseConfiguration parseConfiguration = ParseConfiguration::simple) {
            T t;
            t.parser->prepareParser(parseConfiguration);
            t.parser->parse(argc, argv);
            return t;
        }

        template<ParametersClass T>
        [[nodiscard]] static T parse(const std::vector<std::string>& vals,
                                     ParseConfiguration parseConfiguration = ParseConfiguration::simple) {
            std::string empty;
            return parse<T>(empty, vals, parseConfiguration);
        }

        template<ParametersClass T>
        [[nodiscard]] static T
        parse(const std::string& commandName, const std::vector<std::string>& vals,
              ParseConfiguration parseConfiguration = ParseConfiguration::simple) {
            // guarantee contiguous, null terminated strings
            std::vector<std::vector<char>> vstrings;
            // pointers to those strings
            std::vector<char*> cstrings;
            vstrings.reserve(vals.size() + 1);
            cstrings.reserve(vals.size() + 1);

            vstrings.emplace_back(commandName.begin(), commandName.end());
            vstrings.back().push_back('\0');
            cstrings.push_back(vstrings.back().data());
            for (const auto& val : vals) {
                vstrings.emplace_back(val.begin(), val.end());
                vstrings.back().push_back('\0');
                cstrings.push_back(vstrings.back().data());
            }
            return parse<T>(cstrings.size(), cstrings.data(), parseConfiguration);
        }

        [[nodiscard]]  const std::vector<std::string>& arguments() const {
            return parser->gerParsedArguments();
        }

    };

    template<typename T>
    class BaseParameter {

    protected:
        std::optional<T> value;
    private:
        int counter = 0;

        CommandLineParameters::ArgumentParser::ParseFunc makeParseFunction() {
            return [this](const char* input) {
                std::string text = input != nullptr ? input : "";
                if (!value) {
                    value = fromString<T>(text);
                } else {
                    //if parameter is mentioned multiple times and it's a container, combine options. otherwise, overwrite.
                    if constexpr (is_container_not_string<T>::value) {
                        auto tempValue = fromString<T>(text);
                        std::for_each(tempValue.begin(), tempValue.end(),
                                      [this](auto& i) { value->insert(value->end(), i); });
                    } else {
                        value = fromString<T>(text);
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
        void setValue(const T& v) {
            value = v;
        }

    public:
        BaseParameter(char shortKey, const char* longKey, const char* doc, const char* argumentName,
                      std::optional<T> defaultValue, bool optional, bool hidden) {
            value = defaultValue;
            auto& builder = bsc::CommandLineParameters::parserBuilder();
            builder.addOption(shortKey, longKey, argumentName, makeFlags(optional, hidden), doc, makeParseFunction());
        }

        BaseParameter(char shortKey, const char* longKey, const char* doc, const char* argumentName, bool optional,
                      bool hidden) {
            auto& builder = bsc::CommandLineParameters::parserBuilder();
            builder.addOption(shortKey, longKey, argumentName, makeFlags(optional, hidden), doc, makeParseFunction());
        }

        BaseParameter(char shortKey, const char* doc, const char* argumentName, bool optional, bool hidden) {
            auto& builder = bsc::CommandLineParameters::parserBuilder();
            builder.addOption(shortKey, argumentName, makeFlags(optional, hidden), doc, makeParseFunction());
        }

        BaseParameter(char shortKey, const char* doc, const char* argumentName, std::optional<T> defaultValue,
                      bool optional, bool hidden) {
            value = defaultValue;
            auto& builder = bsc::CommandLineParameters::parserBuilder();
            builder.addOption(shortKey, argumentName, makeFlags(optional, hidden), doc, makeParseFunction());
        }

        BaseParameter(const char* longKey, const char* doc, const char* argumentName, bool optional, bool hidden) {
            auto& builder = bsc::CommandLineParameters::parserBuilder();
            builder.addOption(longKey, argumentName, makeFlags(optional, hidden), doc, makeParseFunction());
        }

        BaseParameter(const char* longKey, const char* doc, const char* argumentName, std::optional<T> defaultValue,
                      bool optional, bool hidden) {
            value = defaultValue;
            auto& builder = bsc::CommandLineParameters::parserBuilder();
            builder.addOption(longKey, argumentName, makeFlags(optional, hidden), doc, makeParseFunction());
        }

        const decltype(value)& operator()() const {
            return value;
        }

        int count() const {
            return counter;
        }


    };

    template<typename T>
    class Parameter : public BaseParameter<T> {

    public:
        Parameter(char shortKey, const char* longKey, const char* argumentName, const char* doc,
                  std::optional<T> defaultValue = std::nullopt) : BaseParameter<T>(shortKey, longKey,
                                                                                   doc, argumentName, defaultValue,
                                                                                   false,
                                                                                   false) {}

        Parameter(char shortKey, const char* argumentName, const char* doc) : BaseParameter<T>(shortKey, argumentName,
                                                                                               doc, false, false) {}

        Parameter(const char* longKey, const char* argumentName, const char* doc) : BaseParameter<T>(longKey,
                                                                                                     argumentName,
                                                                                                     doc, false,
                                                                                                     false) {}


    };

    template<typename T>
    class DefaultParameter : public BaseParameter<T> {

    public:
        DefaultParameter(char shortKey, const char* longKey, const char* argumentName, const char* doc,
                         const T& defaultValue) : BaseParameter<T>(shortKey, longKey,
                                                                   doc, argumentName, defaultValue,
                                                                   false,
                                                                   false) {}

        DefaultParameter(char shortKey, const char* argumentName, const char* doc, const T& defaultValue)
                : BaseParameter<T>(shortKey, doc, argumentName,
                                   nullptr, defaultValue, false, false) {}

        DefaultParameter(const char* longKey, const char* argumentName, const char* doc, const T& defaultValue)
                : BaseParameter<T>(longKey, doc,
                                   argumentName,
                                   defaultValue, false,
                                   false) {}

        const auto& operator()() const {
            return *this->value;
        }


    };

    template<>
    class DefaultParameter<bool> : public BaseParameter<bool> {

    public:
        DefaultParameter(char shortKey, const char* longKey, const char* doc,
                         const bool& defaultValue) : BaseParameter<bool>(shortKey, longKey,
                                                                         doc, nullptr, defaultValue,
                                                                         false,
                                                                         false) {}

        DefaultParameter(char shortKey, const char* doc, const bool& defaultValue)
                : BaseParameter<bool>(shortKey,
                                      doc, nullptr, defaultValue,
                                      false, false) {}

        DefaultParameter(const char* longKey, const char* doc, const bool& defaultValue)
                : BaseParameter<bool>(longKey,
                                      doc, nullptr, defaultValue,
                                      false,
                                      false) {}

        const auto& operator()() const {
            return *this->value;
        }
    };

    template<>
    class Parameter<bool> : public BaseParameter<bool> {
    public:
        Parameter(char shortKey, const char* longKey, const char* doc, std::optional<bool> defaultValue)
                : BaseParameter<bool>(shortKey, longKey, doc, nullptr,
                                      defaultValue, true, false) {

        }

        Parameter(char shortKey, const char* longKey, const char* doc) : BaseParameter<bool>(shortKey, longKey, doc,
                                                                                             nullptr,
                                                                                             true, false) {

        }

        Parameter(char shortKey, const char* doc) : BaseParameter<bool>(shortKey, doc, nullptr, true, false) {

        }

        Parameter(const char* longKey, const char* doc) : BaseParameter<bool>(longKey, doc, nullptr, true, false) {

        }


    };

    template<typename T>
    class OptionalParameter : public BaseParameter<T> {
    public:
        OptionalParameter(char shortKey, const char* longKey, const char* argumentName, const char* doc,
                          const std::optional<T>& defaultValue) : BaseParameter<T>(shortKey, longKey, doc, argumentName,
                                                                                   defaultValue, true, false) {}

        OptionalParameter(char shortKey, const char* argumentName, const char* doc) : BaseParameter<T>(shortKey,
                                                                                                       argumentName,
                                                                                                       doc, true,
                                                                                                       false) {}

        OptionalParameter(const char* longKey, const char* argumentName, const char* doc) : BaseParameter<T>(longKey,
                                                                                                             argumentName,
                                                                                                             doc, true,
                                                                                                             false) {}

    };

    template<typename T>
    class HiddenParameter : public OptionalParameter<T> {

    };

    class Group {

    public:
        Group(const char* doc) {
            auto& builder = CommandLineParameters::parserBuilder();
            builder.addGroup(doc);
        }


    };

    class Alias {
    public:

        Alias(char key) {
            auto& builder = CommandLineParameters::parserBuilder();
            builder.addAlias(key);
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
            builder.addUsage(usage);
        }

        Usage(std::vector<std::string> usage) {
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
            builder.addDoc(doc);
        }
    };


    template<typename T>
    class RequiredParameter : public BaseParameter<T> {
    public:
        RequiredParameter(char shortKey, const char* longKey, const char* argumentName, const char* doc)
                : BaseParameter<T>(
                shortKey, longKey, argumentName, doc,
                false, false) {}

        RequiredParameter(char shortKey, const char* argumentName, const char* doc) : BaseParameter<T>(shortKey,
                                                                                                       argumentName,
                                                                                                       doc, false,
                                                                                                       false) {}

        RequiredParameter(const char* longKey, const char* argumentName, const char* doc) : BaseParameter<T>(longKey,
                                                                                                             argumentName,
                                                                                                             doc, false,
                                                                                                             false) {}
    };


    template<>
    class OptionalParameter<bool> : public BaseParameter<bool> {
    public:
        OptionalParameter(char shortKey, const char* longKey, const char* doc,
                          const std::optional<bool>& defaultValue) : BaseParameter<bool>(shortKey, longKey, doc,
                                                                                         nullptr,

                                                                                         defaultValue, true, false) {}

        OptionalParameter(char shortKey, const char* longKey, const char* doc) : BaseParameter<bool>(shortKey, longKey,
                                                                                                     nullptr, doc, true,
                                                                                                     false) {}

        OptionalParameter(char shortKey, const char* doc) : BaseParameter<bool>(shortKey,
                                                                                nullptr, doc, true, false) {}

        OptionalParameter(const char* longKey, const char* doc) : BaseParameter<bool>(longKey, nullptr, doc, true,
                                                                                      false) {}


    };

    using Flag = Parameter<bool>;
}


#endif //BSC_COMMANDLINEPARAMETERS_H
