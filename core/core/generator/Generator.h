//
// Created by stilgar on 13.12.2019.
//

#ifndef BSC_GENERATOR_H
#define BSC_GENERATOR_H

#include <utility>

namespace bsc {
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
}


#endif //BSC_GENERATOR_H
