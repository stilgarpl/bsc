//
// Created by Krzysztof Tulidowicz on 13.12.2019.
//

#ifndef BSC_RANDOMHASHSTRINGALGORITHM_H
#define BSC_RANDOMHASHSTRINGALGORITHM_H

#include <random>
#include <utility>
#include <string>

namespace bsc {
    class RandomHashStringAlgorithm {

    public:
        using ValueType = std::string;

        ValueType nextValue();
    };
}


#endif //BSC_RANDOMHASHSTRINGALGORITHM_H
