//
// Created by stilgar on 12.11.17.
//

#ifndef BSC_ROLEINVALIDEXCEPTION_H
#define BSC_ROLEINVALIDEXCEPTION_H

#include <stdexcept>

namespace bsc {
    class RoleInvalidException : public std::domain_error {

    public:
        RoleInvalidException(const std::string& arg);
    };
}


#endif //BSC_ROLEINVALIDEXCEPTION_H
