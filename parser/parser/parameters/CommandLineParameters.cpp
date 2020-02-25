//
// Created by stilgar on 07.01.2020.
//

#include <memory>
#include <numeric>
#include "CommandLineParameters.h"


bsc::CommandLineParameters::CommandLineParameters() : parser(parserBuilder().make()) {

}

std::shared_ptr<bsc::CommandLineParameters::ArgumentParser> bsc::CommandLineParameters::ParserBuilder::make() {
    reset();
    return parser;
}

void bsc::CommandLineParameters::ParserBuilder::reset() {
    //reset internal state of builder
    parser = std::make_shared<ArgumentParser>();
    currentKey = 1000;
}

void bsc::CommandLineParameters::ParserBuilder::addOption(char shortKey, const char* longKey, const char* argumentName,
                                                          int flags, const char* doc,
                                                          bsc::CommandLineParameters::ArgumentParser::ParseFunc parserFunction) {
    parser->argpOptions.push_back(argp_option{longKey, shortKey, argumentName, flags, doc, 0});
    parser->parseMap[shortKey] = std::move(parserFunction);
}

void bsc::CommandLineParameters::ParserBuilder::addOption(char shortKey, const char* argumentName, int flags,
                                                          const char* doc,
                                                          bsc::CommandLineParameters::ArgumentParser::ParseFunc parserFunction) {
    parser->argpOptions.push_back(argp_option{nullptr, shortKey, argumentName, flags, doc, 0});
    parser->parseMap[shortKey] = std::move(parserFunction);
}

void bsc::CommandLineParameters::ParserBuilder::addOption(const char* longKey, const char* argumentName, int flags,
                                                          const char* doc,
                                                          bsc::CommandLineParameters::ArgumentParser::ParseFunc parserFunction) {
    auto arg = argp_option{longKey, ++currentKey, argumentName, flags, doc, 0};
    parser->argpOptions.push_back(arg);
    parser->parseMap[arg.key] = std::move(parserFunction);
}

void bsc::CommandLineParameters::ParserBuilder::addGroup(const char* doc) {
    auto arg = argp_option{nullptr, 0, nullptr, 0, doc, 0};
    parser->argpOptions.push_back(arg);
}

void bsc::CommandLineParameters::ParserBuilder::addAlias(char shortKey) {
    auto arg = argp_option{nullptr, shortKey, nullptr, OPTION_ALIAS, nullptr, 0};
    parser->argpOptions.push_back(arg);
}

void bsc::CommandLineParameters::ParserBuilder::addAlias(char shortKey, const char* longKey) {
    auto arg = argp_option{longKey, shortKey, nullptr, OPTION_ALIAS, nullptr, 0};
    parser->argpOptions.push_back(arg);
}

void bsc::CommandLineParameters::ParserBuilder::addAlias(const char* longKey) {
    auto arg = argp_option{longKey, ++currentKey, nullptr, OPTION_ALIAS, nullptr, 0};
    parser->argpOptions.push_back(arg);
}

void bsc::CommandLineParameters::ParserBuilder::addDoc(std::string doc) {
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

void bsc::CommandLineParameters::ParserBuilder::addUsage(std::string usage) {
    parser->usageDocs.push_back(usage);
}

error_t bsc::CommandLineParameters::ArgumentParser::parseArgument(int key, char* arg, struct argp_state* state) {
    auto* self = static_cast<ArgumentParser*>(state->input);
    switch (key) {
        case ARGP_KEY_INIT:
            std::cout << "Parsing ARGP_KEY_INIT " << std::to_string(key) << " " << key << std::endl;
            break;

        case ARGP_KEY_ARG:
            std::cout << "Parsing ARGP_KEY_ARG " << std::to_string(key) << " " << key << std::endl;
            self->parsedArguments.emplace_back(arg);
            {
                auto next = state->next;
                state->next = state->argc;
                while (next < state->argc && state->argv[next]) {
                    self->parsedArguments.emplace_back(state->argv[next]);
                    next++;
                }
            }
            break;

        case ARGP_KEY_END:
            std::cout << "Parsing ARGP_KEY_END " << std::to_string(key) << " " << key << std::endl;
            break;

        case ARGP_KEY_NO_ARGS:
            std::cout << "Parsing ARGP_KEY_NO_ARGS " << std::to_string(key) << " " << key << std::endl;
            break;
        case ARGP_KEY_SUCCESS:
            std::cout << "Parsing ARGP_KEY_SUCCESS " << std::to_string(key) << " " << key << std::endl;
            break;

        case ARGP_KEY_FINI:
            std::cout << "Parsing ARGP_KEY_FINI " << std::to_string(key) << " " << key << std::endl;
            break;
        case ARGP_KEY_ERROR:
            std::cout << "Parsing ARGP_KEY_ERROR " << std::to_string(key) << " " << key << std::endl;
            break;

        default:
            std::cout << "Parsing argument " << (char) key << " " << key << std::endl;
            if (self->parseMap.contains(key)) {
                try {
                    self->parseMap[key](arg);
                } catch (StringParseException& e) {
                    argp_error(state, "Argument \"%s\" parse failed for '%c' with error: [%s]", arg, key, e.what());
                }

            } else {
                return ARGP_ERR_UNKNOWN;
            }
            break;
    }

    return 0;
}

void bsc::CommandLineParameters::ArgumentParser::parse(int argc, char** argv) {

    argp_parse(&argParams, argc, argv, flags, nullptr, this);
}

void bsc::CommandLineParameters::ArgumentParser::prepareParser(ParseConfiguration parseConfiguration) {
    using namespace std::string_literals;
    //close argOptions:
    argpOptions.push_back({nullptr, 0, nullptr, 0, nullptr, 0});
    if (beforeInfo || afterInfo) {
        doc = beforeInfo.value_or(" ") + "\v" + afterInfo.value_or("");
    }


    argDoc = std::accumulate(usageDocs.begin(), usageDocs.end(), ""s, [](const auto& a, const auto& b) {
        if (a.empty()) {
            return b;
        } else {
            return a + "\n" + b;
        }
    });
    flags = ARGP_IN_ORDER;
    switch (parseConfiguration) {
        case ParseConfiguration::simple :
            break;

        case ParseConfiguration::silent:
            flags |= ARGP_SILENT;
            break;
    }

    argParams = {argpOptions.data(), ArgumentParser::parseArgument, argDoc.c_str(), doc.c_str(), nullptr, nullptr, nullptr};
}

char* bsc::CommandLineParameters::ArgumentParser::helpFilter(int key, const char* text, void* input) {
    if (text != nullptr) {
        using namespace std::string_literals;
        //        LOGGER("TEXT FILTER IS : "s + text);
    } else {
        //        LOGGER("TEXT IS NULL");
    }
    return nullptr;
}
