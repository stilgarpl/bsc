//
// Created by stilgar on 09.12.2019.
//

#ifndef BASYCO_CONNECTIONEXCEPTION_H
#define BASYCO_CONNECTIONEXCEPTION_H


#include <stdexcept>

class ConnectionException : public std::domain_error {

public:
    ConnectionException(const std::string& arg);

};


#endif //BASYCO_CONNECTIONEXCEPTION_H
