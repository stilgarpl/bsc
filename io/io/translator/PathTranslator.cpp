//
// Created by stilgar on 21.04.2020.
//

#include "PathTranslator.h"
#include <core/log/Logger.h>
#include <inja/inja.hpp>

namespace bsc {
    namespace detail {}// namespace detail

    fs::path PathTranslator::translate(const std::string& pattern, const PropertiesMetaData& properties) {
        const auto& data = properties;
        try {
            return inja::render(pattern, data);
        } catch (const inja::InjaError& e) {

            PathTranslationException a("Unable to translate pattern with properties. Pattern : " + pattern +
                                       ", properties: " + data.dump(2) + "renderer error" + e.what());
            ERROR(a.what());
            throw a;
        }
    }
    PathTranslationException::PathTranslationException(const std::string& arg) : domain_error(arg) {}
}// namespace bsc