//
// Created by Krzysztof Tulidowicz on 09.07.18.
//

#ifndef BSC_NOTREQUESTEXCEPTION_H
#define BSC_NOTREQUESTEXCEPTION_H

#include <stdexcept>

namespace bsc {
    class NotRequestException : public std::invalid_argument {

    public:
        NotRequestException(const std::string& arg);
    };
}


#endif //BSC_NOTREQUESTEXCEPTION_H
