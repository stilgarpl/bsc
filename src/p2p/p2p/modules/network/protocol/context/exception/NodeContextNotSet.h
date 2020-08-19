//
// Created by Krzysztof Tulidowicz on 11.12.17.
//

#ifndef BSC_NODECONTEXTNOTSET_H
#define BSC_NODECONTEXTNOTSET_H


#include <stdexcept>

namespace bsc {
    class NodeContextNotSet : public std::domain_error {

    public:
        NodeContextNotSet(const std::string& arg);
    };
}


#endif //BSC_NODECONTEXTNOTSET_H
