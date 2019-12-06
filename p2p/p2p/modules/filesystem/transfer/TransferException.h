//
// Created by stilgar on 19.06.19.
//

#ifndef BASYCO_TRANSFEREXCEPTION_H
#define BASYCO_TRANSFEREXCEPTION_H

#include <stdexcept>

//@todo move this somewhere where it belongs
class TransferException : public std::domain_error {

public:
    explicit TransferException(const std::string& arg);
};


#endif //BASYCO_TRANSFEREXCEPTION_H
