//
// Created by stilgar on 13.12.2019.
//

#ifndef BASYCO_GENERATOR_H
#define BASYCO_GENERATOR_H

#include <utility>

template<typename GeneratorAlgorithm>
class Generator {
private:
    GeneratorAlgorithm algorithm;
public:
    using ValueType = typename GeneratorAlgorithm::ValueType;

    auto nextValue() {
        return algorithm.nextValue();
    }

    void reset(ValueType value) {
        algorithm.reset(std::move(value));
    }
};


#endif //BASYCO_GENERATOR_H
