//
// Created by Krzysztof Tulidowicz on 03.02.19.
//

#ifndef BSC_FOREACHCONTEXT_H
#define BSC_FOREACHCONTEXT_H


namespace bsc {
    template<typename T>
    class ForeachContext {

        T t;
    public:
        T& getValue() {
            return t;
        }

        void setValue(const T& t) {
            ForeachContext::t = t;
        }

    };
}


#endif //BSC_FOREACHCONTEXT_H
