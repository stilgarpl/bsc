//
// Created by stilgar on 05.12.2019.
//

#ifndef BASYCO_LOGICEXCEPTIONS_H
#define BASYCO_LOGICEXCEPTIONS_H

#include <stdexcept>

namespace bsc {
    class LogicAssignmentException : public std::domain_error {

    public:
        explicit LogicAssignmentException(const std::string& arg);

        explicit LogicAssignmentException(const char* string);
    };

    class LogicContextInvalid : public std::domain_error {
    public:
        explicit LogicContextInvalid(const std::string& arg);

        explicit LogicContextInvalid(const char* string);
    };
}

#endif //BASYCO_LOGICEXCEPTIONS_H
