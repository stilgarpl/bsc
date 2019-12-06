//
// Created by stilgar on 09.07.18.
//

#ifndef BASYCO_NOTREQUESTEXCEPTION_H
#define BASYCO_NOTREQUESTEXCEPTION_H

#include <stdexcept>

class NotRequestException : public std::invalid_argument {

public:
    NotRequestException(const std::string& arg);
};


#endif //BASYCO_NOTREQUESTEXCEPTION_H
