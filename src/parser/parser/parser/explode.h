//
// Created by Krzysztof Tulidowicz on 15.01.2020.
//

#ifndef BSC_EXPLODE_H
#define BSC_EXPLODE_H

#include <vector>
#include <string>
#include <sstream>

namespace bsc {
    //@todo C++20 constexpr, when stdlib++ has it
    inline std::vector<std::string> explode(const std::string& s, char delim) {
        std::vector<std::string> result;
        std::istringstream iss(s);

        for (std::string token; std::getline(iss, token, delim);) {
            result.push_back(std::move(token));
        }

        return result;
    }
}// namespace bsc

#endif //BSC_EXPLODE_H
