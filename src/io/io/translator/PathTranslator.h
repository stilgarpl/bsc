//
// Created by Krzysztof Tulidowicz on 21.04.2020.
//

#ifndef BSC_PATHTRANSLATOR_H
#define BSC_PATHTRANSLATOR_H

#include "PropertiesMetaData.h"
#include <filesystem>
namespace bsc {
    namespace fs = std::filesystem;
    class PathTranslator {
    public:
        fs::path translate(const std::string& pattern, const PropertiesMetaData& properties);
    };

    class PathTranslationException : public std::domain_error {
    public:
        PathTranslationException(const std::string& arg);
    };
}// namespace bsc

#endif