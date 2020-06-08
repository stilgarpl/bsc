//
// Created by stilgar on 08.06.2020.
//

#ifndef BSC_FILESORTINGEXCEPTIONS_H
#define BSC_FILESORTINGEXCEPTIONS_H

#include <stdexcept>
namespace bsc {
    class FileSortingException : public std::domain_error {
    public:
        FileSortingException(const std::string& arg);
    };
}// namespace bsc

#endif