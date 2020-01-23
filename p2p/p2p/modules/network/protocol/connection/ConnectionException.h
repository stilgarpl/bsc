//
// Created by stilgar on 09.12.2019.
//

#ifndef BSC_CONNECTIONEXCEPTION_H
#define BSC_CONNECTIONEXCEPTION_H


#include <stdexcept>

namespace bsc {
    class ConnectionException : public std::domain_error {

    public:
        ConnectionException(const std::string& arg);

    };
}


#endif //BSC_CONNECTIONEXCEPTION_H
