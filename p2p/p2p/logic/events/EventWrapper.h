//
// Created by stilgar on 30.01.19.
//

#ifndef BASYCO_EVENTWRAPPER_H
#define BASYCO_EVENTWRAPPER_H


#include <p2p/logic/IEvent.h>

template<typename Type>
class EventWrapper : public IEvent<int> {
private:
    Type payload;

public:
    explicit EventWrapper(Type payload) : payload(payload) {}

    Type &getPayload() {
        return payload;
    }

    operator Type &() {
        return payload;
    }

    operator Type() {
        return payload;
    }
};


#endif //BASYCO_EVENTWRAPPER_H