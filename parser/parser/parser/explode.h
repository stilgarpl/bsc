//
// Created by stilgar on 15.01.2020.
//

#ifndef BASYCO_EXPLODE_H
#define BASYCO_EXPLODE_H

#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> explode(std::string const& s, char delim) {
    std::vector<std::string> result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim);) {
        result.push_back(std::move(token));
    }

    return result;
}

#endif //BASYCO_EXPLODE_H
