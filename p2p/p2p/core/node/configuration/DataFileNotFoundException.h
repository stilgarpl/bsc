//
// Created by stilgar on 31.05.19.
//

#ifndef BSC_DATAFILENOTFOUNDEXCEPTION_H
#define BSC_DATAFILENOTFOUNDEXCEPTION_H

#include <stdexcept>

namespace bsc {
    class DataFileNotFoundException : public std::domain_error {

    public:
        explicit DataFileNotFoundException(const std::string& arg);
    };
}


#endif //BSC_DATAFILENOTFOUNDEXCEPTION_H
