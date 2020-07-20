//
// Created by Krzysztof Tulidowicz on 12.12.2019.
//

#ifndef BSC_GETSET_H
#define BSC_GETSET_H

namespace bsc {
    template<typename T>
    class GetSet {
        T& obj;
    public:
        explicit GetSet(T& o) : obj(o) {};

        operator const T&() const {
            return obj;
        }

        GetSet& operator=(const T& other) {
            obj = other;
            return *this;
        }


    };
}


#endif //BSC_GETSET_H
