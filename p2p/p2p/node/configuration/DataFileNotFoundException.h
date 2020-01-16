//
// Created by stilgar on 31.05.19.
//

#ifndef BASYCO_DATAFILENOTFOUNDEXCEPTION_H
#define BASYCO_DATAFILENOTFOUNDEXCEPTION_H

#include <stdexcept>

namespace bsc {
    class DataFileNotFoundException : public std::domain_error {

    public:
        explicit DataFileNotFoundException(const std::string& arg);
    };
}


#endif //BASYCO_DATAFILENOTFOUNDEXCEPTION_H
