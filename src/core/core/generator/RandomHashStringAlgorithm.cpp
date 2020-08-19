//
// Created by Krzysztof Tulidowicz on 13.12.2019.
//

#include "RandomHashStringAlgorithm.h"
#include <core/utils/crypto.h>

bsc::RandomHashStringAlgorithm::ValueType bsc::RandomHashStringAlgorithm::nextValue() {
    return calculateSha1OfString(std::to_string(std::chrono::system_clock::now().time_since_epoch().count()));
}
