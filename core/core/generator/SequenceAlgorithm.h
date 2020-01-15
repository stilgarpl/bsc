//
// Created by stilgar on 13.12.2019.
//

#ifndef BASYCO_SEQUENCEALGORITHM_H
#define BASYCO_SEQUENCEALGORITHM_H

#include <utility>

namespace bsc {
    template<typename IntegerType>
    struct SequenceAlgorithm {
    public:
        using ValueType = IntegerType;

    private:
        IntegerType currentValue = 0;
    public:
        void reset(IntegerType newValue) {
            currentValue = std::move(newValue);
        }

        auto nextValue() {
            return ++currentValue;
        }

    };
}


#endif //BASYCO_SEQUENCEALGORITHM_H
