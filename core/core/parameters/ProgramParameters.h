//
// Created by stilgar on 07.01.2020.
//

#ifndef BASYCO_PROGRAMPARAMETERS_H
#define BASYCO_PROGRAMPARAMETERS_H


#include <optional>
#include <functional>
#include <argp.h>
#include <iostream>
#include <map>
#include <utility>
#include <memory>
#include "core/parser/FromString.h"


class ProgramParameters;

template<typename T>
concept ParametersClass = std::is_base_of_v<ProgramParameters, T>;

class ProgramParameters {
private:
    class ParserBuilder;

    class Parser {
    public:
        using ParseFunc = std::function<void(const char*)>;
    private:
        std::vector<argp_option> argpOptions;
        std::string doc;
        std::string argDoc;
        argp argParams;
        std::map<decltype(argp_option::key), ParseFunc> parseMap;
        std::vector<std::string> parsedArguments;

    public:
        static error_t parseArgument(int key, char* arg, struct argp_state* state);

        void
        prepareParser(std::vector<std::string> usage, const std::optional<std::string>& before,
                      const std::optional<std::string>& after);

        void parse(int argc, char* argv[]);


        static char* helpFilter(int key, const char* text, void* input);

        auto& gerParsedArguments() {
            return parsedArguments;
        }

        friend class ProgramParameters::ParserBuilder;


    };

    class ParserBuilder {
    private:
        std::shared_ptr<Parser> parser = nullptr;
        int currentKey = 0;
    public:
        void addOption(char shortKey, const char* longKey, const char* argumentName, int flags, const char* doc,
                       Parser::ParseFunc parserFunction) {
            parser->argpOptions.push_back(argp_option{longKey, shortKey, argumentName, flags, doc, 0});
            parser->parseMap[shortKey] = std::move(parserFunction);
        }

        void addOption(char shortKey, const char* argumentName, int flags, const char* doc,
                       Parser::ParseFunc parserFunction) {
            parser->argpOptions.push_back(argp_option{nullptr, shortKey, argumentName, flags, doc, 0});
            parser->parseMap[shortKey] = std::move(parserFunction);
        }

        void
        addOption(const char* longKey, const char* argumentName, int flags, const char* doc,
                  Parser::ParseFunc parserFunction) {
            auto arg = argp_option{longKey, ++currentKey, argumentName, flags, doc, 0};
            parser->argpOptions.push_back(arg);
            parser->parseMap[arg.key] = std::move(parserFunction);
        }

        void addGroup(const char* doc) {
            auto arg = argp_option{nullptr, 0, nullptr, 0, doc, 0};
            parser->argpOptions.push_back(arg);
        }

        void addAlias(char shortKey) {
            auto arg = argp_option{nullptr, shortKey, nullptr, OPTION_ALIAS, nullptr, 0};
            parser->argpOptions.push_back(arg);
        }

        void addAlias(char shortKey, const char* longKey) {
            auto arg = argp_option{longKey, shortKey, nullptr, OPTION_ALIAS, nullptr, 0};
            parser->argpOptions.push_back(arg);
        }

        void addAlias(const char* longKey) {
            auto arg = argp_option{longKey, ++currentKey, nullptr, OPTION_ALIAS, nullptr, 0};
            parser->argpOptions.push_back(arg);
        }

        std::shared_ptr<Parser> make();

        void reset();
    };

    static ParserBuilder& parserBuilder() {
        static thread_local ParserBuilder parserBuilder;
        return parserBuilder;
    }


    const std::shared_ptr<Parser> parser;


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

    friend class Alias;

public:
    ProgramParameters();

    struct DocsAndInfo {
        std::vector<std::string> usageDocs;
        std::optional<std::string> beforeInfo;
        std::optional<std::string> afterInfo;
    };

    template<ParametersClass T>
    [[nodiscard]] static T parse(int argc, char* argv[], DocsAndInfo docsAndInfo = {}) {
        T t;
        t.parser->prepareParser(docsAndInfo.usageDocs, docsAndInfo.beforeInfo, docsAndInfo.afterInfo);
        t.parser->parse(argc, argv);
        return t;
    }

    template<ParametersClass T>
    [[nodiscard]] static T parse(const std::vector<std::string>& vals) {
        std::string empty;
        return parse<T>(empty, vals);
    }

    template<ParametersClass T>
    [[nodiscard]] static T parse(const std::string& commandName, const std::vector<std::string>& vals) {
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
        return parse<T>(cstrings.size(), cstrings.data());
    }

//    void usage(std::string usg) {
//        usageDocs.push_back(usg);
//    }
//
//    void before(std::string b) {
//        beforeInfo = b;
//    }
//
//    void after(std::string a) {
//        afterInfo = a;
//    }

    [[nodiscard]]  const std::vector<std::string>& arguments() const {
        return parser->gerParsedArguments();
    }

};


template<typename T>
class BaseParameter {
    //@todo argumentName could have default name VALUE or sth.
protected:
    std::optional<T> value;
private:
    int counter = 0;

    ProgramParameters::Parser::ParseFunc makeParseFunction() {
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
    BaseParameter(char shortKey, const char* longKey, const char* argumentName, const char* doc,
                  std::optional<T> defaultValue, bool optional, bool hidden) {
        value = defaultValue;
        auto& builder = ProgramParameters::parserBuilder();
        builder.addOption(shortKey, longKey, argumentName, makeFlags(optional, hidden), doc, makeParseFunction());
    }

    BaseParameter(char shortKey, const char* longKey, const char* doc, const char* argumentName, bool optional,
                  bool hidden) {
        auto& builder = ProgramParameters::parserBuilder();
        builder.addOption(shortKey, longKey, argumentName, makeFlags(optional, hidden), doc, makeParseFunction());
    }

    BaseParameter(char shortKey, const char* doc, const char* argumentName, bool optional, bool hidden) {
        auto& builder = ProgramParameters::parserBuilder();
        builder.addOption(shortKey, argumentName, makeFlags(optional, hidden), doc, makeParseFunction());
    }

    BaseParameter(const char* longKey, const char* doc, const char* argumentName, bool optional, bool hidden) {
        auto& builder = ProgramParameters::parserBuilder();
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
                                                                               argumentName, doc, defaultValue, false,
                                                                               false) {}

    Parameter(char shortKey, const char* argumentName, const char* doc) : BaseParameter<T>(shortKey, argumentName,
                                                                                           doc, false, false) {}

    Parameter(const char* longKey, const char* argumentName, const char* doc) : BaseParameter<T>(longKey, argumentName,
                                                                                                 doc, false, false) {}


};

template<>
class Parameter<bool> : public BaseParameter<bool> {
public:
    Parameter(char shortKey, const char* longKey, const char* doc, std::optional<bool> defaultValue)
            : BaseParameter<bool>(shortKey, longKey, nullptr,
                                  doc, defaultValue, true, false) {

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

using Flag = Parameter<bool>;


template<typename T>
class OptionalParameter : public BaseParameter<T> {
public:
    OptionalParameter(char shortKey, const char* longKey, const char* argumentName, const char* doc,
                      const std::optional<T>& defaultValue) : BaseParameter<T>(shortKey, longKey, argumentName, doc,
                                                                               defaultValue, true, false) {}

    OptionalParameter(char shortKey, const char* argumentName, const char* doc) : BaseParameter<T>(shortKey,
                                                                                                   argumentName,
                                                                                                   doc, true, false) {}

    OptionalParameter(const char* longKey, const char* argumentName, const char* doc) : BaseParameter<T>(longKey,
                                                                                                         argumentName,
                                                                                                         doc, true,
                                                                                                         false) {}

};

template<>
class OptionalParameter<bool> : public BaseParameter<bool> {
public:
    OptionalParameter(char shortKey, const char* longKey, const char* doc,
                      const std::optional<bool>& defaultValue) : BaseParameter<bool>(shortKey, longKey, nullptr, doc,
                                                                                     defaultValue, true, false) {}

    OptionalParameter(char shortKey, const char* longKey, const char* doc) : BaseParameter<bool>(shortKey, longKey,
                                                                                                 nullptr, doc, true,
                                                                                                 false) {}

    OptionalParameter(char shortKey, const char* doc) : BaseParameter<bool>(shortKey,
                                                                            nullptr, doc, true, false) {}

    OptionalParameter(const char* longKey, const char* doc) : BaseParameter<bool>(longKey, nullptr, doc, true, false) {}


};

template<typename T>
class RequiredParameter : public BaseParameter<T> {
public:
    RequiredParameter(char shortKey, const char* longKey, const char* argumentName, const char* doc) : BaseParameter<T>(
            shortKey, longKey, argumentName, doc,
            false, false) {}

    RequiredParameter(char shortKey, const char* argumentName, const char* doc) : BaseParameter<T>(shortKey,
                                                                                                   argumentName,
                                                                                                   doc, false, false) {}

    RequiredParameter(const char* longKey, const char* argumentName, const char* doc) : BaseParameter<T>(longKey,
                                                                                                         argumentName,
                                                                                                         doc, false,
                                                                                                         false) {}
};

template<typename T>
class HiddenParameter : public OptionalParameter<T> {

};

class Group {

public:
    Group(const char* doc) {
        auto& builder = ProgramParameters::parserBuilder();
        builder.addGroup(doc);
    }


};

class Alias {
public:

    Alias(char key) {
        auto& builder = ProgramParameters::parserBuilder();
        builder.addAlias(key);
    }

    Alias(const char* longKey) {
        auto& builder = ProgramParameters::parserBuilder();
        builder.addAlias(longKey);
    }

    Alias(char key, const char* longKey) {
        auto& builder = ProgramParameters::parserBuilder();
        builder.addAlias(key, longKey);
    }
};

#endif //BASYCO_PROGRAMPARAMETERS_H
