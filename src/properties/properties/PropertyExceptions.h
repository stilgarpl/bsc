//
// Created by Krzysztof Tulidowicz on 24.01.2021.
//

#ifndef BSC_PROPERTYEXCEPTIONS_H
#define BSC_PROPERTYEXCEPTIONS_H

namespace bsc {
    class InvalidPropertyKeyException : public std::domain_error {
    public:
        explicit InvalidPropertyKeyException(const std::string& arg) : domain_error(arg) {}
    };

    class InvalidPropertyTypeException : public std::domain_error {
    public:
        explicit InvalidPropertyTypeException(const std::string& arg) : domain_error(arg) {}
    };

    class PropertyContextAlreadyLoaded : public std::domain_error {
    public:
        explicit PropertyContextAlreadyLoaded(const std::string& arg) : domain_error(arg) {}
    };

}// namespace bsc

#endif// BSC_PROPERTYEXCEPTIONS_H
