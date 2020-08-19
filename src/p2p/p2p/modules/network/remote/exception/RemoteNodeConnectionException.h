//
// Created by Krzysztof Tulidowicz on 10.06.19.
//

#ifndef BSC_REMOTENODECONNECTIONEXCEPTION_H
#define BSC_REMOTENODECONNECTIONEXCEPTION_H

#include <stdexcept>

namespace bsc {
    class RemoteNodeConnectionException : public std::domain_error {

    public:
        RemoteNodeConnectionException(const std::string& arg);

    };
}


#endif //BSC_REMOTENODECONNECTIONEXCEPTION_H
