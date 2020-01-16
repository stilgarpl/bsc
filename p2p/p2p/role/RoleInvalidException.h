//
// Created by stilgar on 12.11.17.
//

#ifndef BASYCO_ROLEINVALIDEXCEPTION_H
#define BASYCO_ROLEINVALIDEXCEPTION_H

#include <stdexcept>

namespace bsc {
    class RoleInvalidException : public std::domain_error {

    public:
        RoleInvalidException(const std::string& arg);
    };
}


#endif //BASYCO_ROLEINVALIDEXCEPTION_H
