//
// Created by Krzysztof Tulidowicz on 21.04.2020.
//

#include "PathTranslator.h"
#include <inja/inja.hpp>
#include <log/log/Logger.h>

namespace bsc {
    namespace detail {}// namespace detail

    fs::path PathTranslator::translate(const std::string& pattern, const PropertiesMetaData& properties) const {
        const auto& data = properties;
        try {
            return inja::render(pattern, data);
        } catch (const inja::InjaError& e) {

            PathTranslationException a("Unable to translate pattern with properties. Pattern : " + pattern +
                                       ", properties: " + data.dump(2) + "renderer error" + e.what());
            logger.error(a.what());
            throw a;
        }
    }
    PathTranslationException::PathTranslationException(const std::string& arg) : domain_error(arg) {}
}// namespace bsc