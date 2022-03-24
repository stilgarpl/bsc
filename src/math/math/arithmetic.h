//
// Created by Krzysztof Tulidowicz on 24.03.2022.
//
#ifndef BSC_ARITHMETIC_H
#define BSC_ARITHMETIC_H

namespace bsc {
    namespace math {
        template<typename T>
        static constexpr inline T pown(T x, unsigned p) {
            T result = 1;

            while (p) {
                if (p & 0x1) {
                    result *= x;
                }
                x *= x;
                p >>= 1;
            }

            return result;
        }
    }
}

#endif// BSC_ARITHMETIC_H
