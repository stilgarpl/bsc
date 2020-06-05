//
// Created by stilgar on 07.01.2020.
//

#include "CommandLineParameters.h"
#include <memory>
#include <numeric>
#include <utility>
namespace bsc {
    CommandLineParameters::CommandLineParameters() : parser(parserBuilder().make()) {}

    std::shared_ptr<CommandLineParameters::ArgumentParser> CommandLineParameters::ParserBuilder::make() {
        reset();
        return parser;
    }

    void CommandLineParameters::ParserBuilder::reset() {
        // reset internal state of builder
        parser     = std::make_shared<ArgumentParser>();
        currentKey = 1000;
    }

    void CommandLineParameters::ParserBuilder::addOption(
            CommandLineParameters::ParserBuilder::ParserOptions parserOptions,
            CommandLineParameters::ArgumentParser::OptionParseFunc parserFunction) {
        // if there is no argument name provided, then argument will be optional
        int flags = parserOptions.argumentName.has_value() ? parserOptions.flags
                                                           : parserOptions.flags | OPTION_ARG_OPTIONAL;
        auto arg = argp_option{.name  = parserOptions.longKey ? parserOptions.longKey->data() : nullptr,
                               .key   = parserOptions.shortKey ? *parserOptions.shortKey : ++currentKey,
                               .arg   = parserOptions.argumentName ? parserOptions.argumentName->data() : nullptr,
                               .flags = flags,
                               .doc   = parserOptions.doc ? parserOptions.doc->data() : nullptr,
                               .group = 0};
        parser->argpOptions.push_back(arg);
        parser->parseMap[arg.key] = std::move(parserFunction);
    }

    void CommandLineParameters::ParserBuilder::addGroup(const char* doc) {
        auto arg = argp_option{nullptr, 0, nullptr, 0, doc, 0};
        parser->argpOptions.push_back(arg);
    }

    void CommandLineParameters::ParserBuilder::addAlias(char shortKey) {
        auto arg = argp_option{nullptr, shortKey, nullptr, OPTION_ALIAS, nullptr, 0};
        parser->argpOptions.push_back(arg);
    }

    void CommandLineParameters::ParserBuilder::addAlias(char shortKey, const char* longKey) {
        auto arg = argp_option{longKey, shortKey, nullptr, OPTION_ALIAS, nullptr, 0};
        parser->argpOptions.push_back(arg);
    }

    void CommandLineParameters::ParserBuilder::addAlias(const char* longKey) {
        auto arg = argp_option{longKey, ++currentKey, nullptr, OPTION_ALIAS, nullptr, 0};
        parser->argpOptions.push_back(arg);
    }

    void CommandLineParameters::ParserBuilder::addDoc(std::string doc) {
        if (parser->argpOptions.size() == 0) {
            auto& before = parser->beforeInfo;
            if (before.has_value()) {
                *before += "\n" + doc;
            } else {
                before = doc;
            }
        } else {
            auto& after = parser->afterInfo;
            if (after.has_value()) {
                *after += "\n" + doc;
            } else {
                after = doc;
            }
        }
    }

    void CommandLineParameters::ParserBuilder::addUsage(std::string usage) {
        parser->usageDocs.push_back(std::move(usage));
    }
    void CommandLineParameters::ParserBuilder::addArgument(ArgumentParser::ArgumentParseFunc parserFunction,
                                                           std::optional<std::string> argumentName) {
        parser->argumentDescriptors.push_back({.argumentParseFunc = std::move(parserFunction),
                                               .argumentIndex     = parser->argumentDescriptors.size(),
                                               .argumentName      = std::move(argumentName)});
        parser->incrementRequiredArguments();
    }

    error_t CommandLineParameters::ArgumentParser::parseArgument(int key, char* arg, struct argp_state* state) {
        auto* self = static_cast<ArgumentParser*>(state->input);
        switch (key) {
            case ARGP_KEY_INIT:
                //            std::cout << "Parsing ARGP_KEY_INIT " << std::to_string(key) << " " << key << std::endl;
                break;

            case ARGP_KEY_ARG:
                //            std::cout << "Parsing ARGP_KEY_ARG " << std::to_string(key) << " " << key << std::endl;
                self->rawArguments.emplace_back(arg);
                {
                    auto next   = state->next;
                    state->next = state->argc;
                    while (next < state->argc && state->argv[next]) {
                        self->rawArguments.emplace_back(state->argv[next]);
                        next++;
                    }
                }
                break;

            case ARGP_KEY_END:
                if (self->requiredArgumentsCount.has_value()) {
                    if (self->requiredArgumentsCount.value() > self->rawArguments.size()) {
                        argp_error(state,
                                   "Arguments required: %zu, got %zu",
                                   *self->requiredArgumentsCount,
                                   self->rawArguments.size());
                    }
                }
                break;

            case ARGP_KEY_NO_ARGS:
                //            std::cout << "Parsing ARGP_KEY_NO_ARGS " << std::to_string(key) << " " << key <<
                //            std::endl;
                break;
            case ARGP_KEY_SUCCESS:
                //            std::cout << "Parsing ARGP_KEY_SUCCESS " << std::to_string(key) << " " << key <<
                //            std::endl;
                break;

            case ARGP_KEY_FINI:
                //            std::cout << "Parsing ARGP_KEY_FINI " << std::to_string(key) << " " << key << std::endl;
                break;
            case ARGP_KEY_ERROR:
                //            std::cout << "Parsing ARGP_KEY_ERROR " << std::to_string(key) << " " << key << std::endl;
                break;

            default:
                //            std::cout << "Parsing argument " << (char) key << " " << key << std::endl;
                if (self->parseMap.contains(key)) {
                    try {
                        self->parseMap[key](arg);
                    } catch (StringParseException& e) {
                        if (self->parseConfiguration == ParseConfiguration::simple) {
                            argp_error(state,
                                       "Argument \"%s\" parse failed for '%c' with error: [%s]",
                                       arg,
                                       key,
                                       e.what());
                        } else {
                            throw e;
                        }
                    } catch (ValueNotAllowed& e) {
                        if (self->parseConfiguration == ParseConfiguration::simple) {
                            using namespace std::string_literals;
                            std::string allowedValues = std::accumulate(
                                    e.allowedValues.begin(),
                                    e.allowedValues.end(),
                                    ""s,
                                    [](const auto& a, const auto& b) { return a.empty() ? b : a + ", " + b; });
                            //@todo add option name (short or long) to message.
                            argp_error(state,
                                       "Value \"%s\" is not allowed. Allowed values: %s",
                                       arg,
                                       allowedValues.c_str());
                        } else {
                            throw e;
                        }
                    }

                } else {
                    return ARGP_ERR_UNKNOWN;
                }
                break;
        }

        return 0;
    }

    void CommandLineParameters::ArgumentParser::parse(int argc, char** argv) {

        argp_parse(&argParams, argc, argv, flags, nullptr, this);
        parseNamedArguments();
    }

    void CommandLineParameters::ArgumentParser::prepareParser(ParseConfiguration parseConfiguration) {
        using namespace std::string_literals;
        // close argOptions:
        argpOptions.push_back({nullptr, 0, nullptr, 0, nullptr, 0});
        if (beforeInfo || afterInfo) {
            doc = beforeInfo.value_or(" ") + "\v" + afterInfo.value_or("");
        }

        prepareArgumentUsage();

        argDoc = std::accumulate(usageDocs.begin(), usageDocs.end(), ""s, [](const auto& a, const auto& b) {
            if (a.empty()) {
                return b;
            } else {
                return a + "\n" + b;
            }
        });
        flags  = ARGP_IN_ORDER;
        switch (parseConfiguration) {
            case ParseConfiguration::simple:
                break;

            case ParseConfiguration::silent:
                flags |= ARGP_SILENT;
                break;
        }
        this->parseConfiguration = parseConfiguration;
        argParams                = {argpOptions.data(),
                     ArgumentParser::parseArgument,
                     argDoc.c_str(),
                     doc.c_str(),
                     nullptr,
                     nullptr,
                     nullptr};
    }

    char* CommandLineParameters::ArgumentParser::helpFilter(int key, const char* text, void* input) {
        if (text != nullptr) {
            using namespace std::string_literals;
            //        LOGGER("TEXT FILTER IS : "s + text);
        } else {
            //        LOGGER("TEXT IS NULL");
        }
        return nullptr;
    }
    void CommandLineParameters::ArgumentParser::parseNamedArguments() {
        for (const auto& descriptor : argumentDescriptors) {
            if (descriptor.argumentIndex < rawArguments.size()) {
                descriptor.argumentParseFunc(rawArguments[descriptor.argumentIndex]);
            }
        }
    }
    void CommandLineParameters::ArgumentParser::prepareArgumentUsage() {
        static const std::string defaultArgumentName = "ARG#";
        std::string usageString{};
        for (const auto& descriptor : argumentDescriptors) {
            if (descriptor.argumentName.has_value()) {
                usageString += *descriptor.argumentName;
            } else {
                usageString += defaultArgumentName + std::to_string(descriptor.argumentIndex + 1);
            }
            usageString += " ";
        }
        if (usageString.size() > 1) usageString.pop_back();
        usageDocs.insert(usageDocs.begin(), usageString);
    }

    ValueNotAllowed::ValueNotAllowed(const std::string& arg) : ValueNotAllowed(arg, {}) {}
    ValueNotAllowed::ValueNotAllowed(const std::string& arg, std::remove_cvref_t<decltype(allowedValues)> a)
        : domain_error(arg), allowedValues(std::move(a)) {}
}// namespace bsc