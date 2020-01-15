//
// Created by stilgar on 12.12.2019.
//

#ifndef BASYCO_GETSET_H
#define BASYCO_GETSET_H

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


#endif //BASYCO_GETSET_H
