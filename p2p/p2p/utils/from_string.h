//
// Created by stilgar on 16.11.17.
//

#ifndef BASYCO_FROM_STRING_H
#define BASYCO_FROM_STRING_H


#include <cstdlib>
#include <string>


template<typename T, typename S>
inline T from_string(const S &s) {
    return T(s);
}

template<>
inline int from_string<int>(const std::string &s) {
    return std::atoi(s.c_str());
}

template<>
inline float from_string<float>(const std::string &s) {
    return static_cast<float>(std::atof(s.c_str()));
}


#endif //BASYCO_FROM_STRING_H
