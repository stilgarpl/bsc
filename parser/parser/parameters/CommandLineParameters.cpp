//
// Created by stilgar on 07.01.2020.
//

#include <memory>
#include <numeric>
#include "CommandLineParameters.h"


bsc::CommandLineParameters::CommandLineParameters() : parser(parserBuilder().make()) {

}

std::shared_ptr<bsc::CommandLineParameters::Parser> bsc::CommandLineParameters::ParserBuilder::make() {
    reset();
    return parser;
}

void bsc::CommandLineParameters::ParserBuilder::reset() {
    //reset internal state of builder
    parser = std::make_shared<Parser>();
    currentKey = 1000;

}

error_t bsc::CommandLineParameters::Parser::parseArgument(int key, char* arg, struct argp_state* state) {
    auto* self = static_cast<Parser*>(state->input);
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

void bsc::CommandLineParameters::Parser::parse(int argc, char** argv) {

    argp_parse(&argParams, argc, argv, flags, nullptr, this);
}

void
bsc::CommandLineParameters::Parser::prepareParser(std::vector<std::string> usage,
                                                  const std::optional<std::string>& before,
                                                  const std::optional<std::string>& after, bool exitOnFailure,
                                                  bool silent) {
    using namespace std::string_literals;
    //close argOptions:
    argpOptions.push_back({nullptr, 0, nullptr, 0, nullptr, 0});
    if (before || after) {
        doc = before.value_or(" ") + "\v" + after.value_or("");
    }


    argDoc = std::accumulate(usage.begin(), usage.end(), ""s, [](const auto& a, const auto& b) {
        if (a.empty()) {
            return b;
        } else {
            return a + "\n" + b;
        }
    });
    flags = ARGP_IN_ORDER;
    if (!exitOnFailure) {
        flags |= ARGP_NO_EXIT;
    }
    if (silent) {
        flags |= ARGP_SILENT;
    }

    argParams = {argpOptions.data(), Parser::parseArgument, argDoc.c_str(), doc.c_str(), nullptr, nullptr, nullptr};
}

char* bsc::CommandLineParameters::Parser::helpFilter(int key, const char* text, void* input) {
    if (text != nullptr) {
        using namespace std::string_literals;
//        LOGGER("TEXT FILTER IS : "s + text);
    } else {
//        LOGGER("TEXT IS NULL");
    }
    return nullptr;
}

