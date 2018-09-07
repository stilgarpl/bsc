//
// Created by stilgar on 20.07.18.
//

#ifndef BASYCO_LOGICSTATEMACHINE_H
#define BASYCO_LOGICSTATEMACHINE_H


#include <p2p/log/Logger.h>
#include "StateMachine.h"

template<typename StateIdType>
class LogicStateMachine : public StateMachine<StateIdType> {
public:


    static void onEnter(const StateIdType &id) {
        LOGGER("LOGIC MACHINE ON ENTER" + std::to_string(id));
    }

    static void onLeave(const StateIdType &id) {
        LOGGER("LOGIC MACHINE ON LEAVE" + std::to_string(id));
    }

    LogicStateMachine() : StateMachine<StateIdType>(onEnter, onLeave) {};

};


#endif //BASYCO_LOGICSTATEMACHINE_H
