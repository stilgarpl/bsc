//
// Created by Krzysztof Tulidowicz on 07.07.18.
//

#ifndef BSC_REMOTENODENOTFOUNDEXCEPTION_H
#define BSC_REMOTENODENOTFOUNDEXCEPTION_H

#include <stdexcept>

namespace bsc {
    class RemoteNodeNotFoundException : public std::domain_error {

    public:
        RemoteNodeNotFoundException(const std::string& arg);
    };
}


#endif //BSC_REMOTENODENOTFOUNDEXCEPTION_H
