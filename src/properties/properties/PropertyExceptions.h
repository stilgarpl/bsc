//
// Created by Krzysztof Tulidowicz on 24.01.2021.
//

#ifndef BSC_PROPERTYEXCEPTIONS_H
#define BSC_PROPERTYEXCEPTIONS_H

namespace bsc {
    class InvalidPropertyKeyException : public std::domain_error {
    public:
        InvalidPropertyKeyException(const std::string& arg);
    };

    class InvalidPropertyTypeException : public std::domain_error {
    public:
        InvalidPropertyTypeException(const std::string& arg);
    };

}// namespace bsc

#endif// BSC_PROPERTYEXCEPTIONS_H
