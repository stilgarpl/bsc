//
// Created by Krzysztof Tulidowicz on 05.12.2019.
//

#ifndef BSC_LOGICEXCEPTIONS_H
#define BSC_LOGICEXCEPTIONS_H

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

#endif //BSC_LOGICEXCEPTIONS_H
