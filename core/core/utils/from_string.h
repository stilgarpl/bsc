//
// Created by stilgar on 16.11.17.
//

#ifndef BASYCO_FROM_STRING_H
#define BASYCO_FROM_STRING_H


#include <string>


template<typename T, typename S>
inline typename std::remove_reference<T>::type from_string(const S &s) {
    return typename std::remove_reference<T>::type(s);
}

template<>
inline int from_string<int>(const std::string &s) {
    return std::stoi(s);
}

template<>
inline long from_string<long>(const std::string &s) {
    return std::stol(s);
}

template<>
inline float from_string<float>(const std::string &s) {
    return static_cast<float>(std::stof(s));
}


#endif //BASYCO_FROM_STRING_H
