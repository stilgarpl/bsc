//
// Created by stilgar on 07.07.18.
//

#ifndef BASYCO_REMOTENODENOTFOUNDEXCEPTION_H
#define BASYCO_REMOTENODENOTFOUNDEXCEPTION_H

#include <stdexcept>

class RemoteNodeNotFoundException : public std::domain_error {

public:
    RemoteNodeNotFoundException(const std::string& arg);
};


#endif //BASYCO_REMOTENODENOTFOUNDEXCEPTION_H
