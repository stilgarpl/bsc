//
// Created by Krzysztof Tulidowicz on 19.06.19.
//

#ifndef BSC_TRANSFEREXCEPTION_H
#define BSC_TRANSFEREXCEPTION_H

#include <stdexcept>

namespace bsc {
    //@todo move this somewhere where it belongs
    class TransferException : public std::domain_error {

    public:
        explicit TransferException(const std::string& arg);
    };
}


#endif //BSC_TRANSFEREXCEPTION_H
