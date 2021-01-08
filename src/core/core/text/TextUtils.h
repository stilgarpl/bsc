//
// Created by Krzysztof Tulidowicz on 30.10.2020.
//

#ifndef BSC_TEXTUTILS_H
#define BSC_TEXTUTILS_H

#include <regex>
#include <string>
namespace bsc {
    class TextUtils {
    public:
        static std::string escapeAllRegexCharacters(const std::string& input) {
            std::regex specialCharacters(R"(\.|\(|\)|\[|\])");
            return std::regex_replace(input, specialCharacters, "\\$&");
        }
    };
}// namespace bsc

#endif