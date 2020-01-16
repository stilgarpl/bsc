//
// Created by stilgar on 10.06.19.
//

#ifndef BASYCO_REMOTENODECONNECTIONEXCEPTION_H
#define BASYCO_REMOTENODECONNECTIONEXCEPTION_H

#include <stdexcept>

namespace bsc {
    class RemoteNodeConnectionException : public std::domain_error {

    public:
        RemoteNodeConnectionException(const std::string& arg);

    };
}


#endif //BASYCO_REMOTENODECONNECTIONEXCEPTION_H
