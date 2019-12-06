//
// Created by stilgar on 03.02.19.
//

#ifndef BASYCO_FOREACHCONTEXT_H
#define BASYCO_FOREACHCONTEXT_H


template<typename T>
class ForeachContext {

    T t;
public:
    T& getValue() {
        return t;
    }

    void setValue(const T &t) {
        ForeachContext::t = t;
    }

};


#endif //BASYCO_FOREACHCONTEXT_H
