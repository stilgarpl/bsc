//
// Created by stilgar on 13.12.2019.
//

#ifndef BASYCO_RANDOMHASHSTRINGALGORITHM_H
#define BASYCO_RANDOMHASHSTRINGALGORITHM_H

#include <random>
#include <utility>
#include <string>

class RandomHashStringAlgorithm {

public:
    using ValueType = std::string;

    ValueType nextValue();
};


#endif //BASYCO_RANDOMHASHSTRINGALGORITHM_H
