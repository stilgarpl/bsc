//
// Created by stilgar on 13.12.2019.
//

#ifndef BASYCO_RANDOMALGORITHM_H
#define BASYCO_RANDOMALGORITHM_H

#include <random>
#include <utility>

namespace bsc {
    class RandomAlgorithm {
    private:
        std::random_device randomDevice;
    public:
        using ValueType = std::random_device::result_type;

        auto nextValue() {
            return randomDevice();
        }

        static void reset(ValueType newValue) {
            //do nothing, can't reset random device.
        }


    };
}


#endif //BASYCO_RANDOMALGORITHM_H
