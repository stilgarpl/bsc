//
// Created by stilgar on 20.07.18.
//

#ifndef BASYCO_LOGICSTATEMACHINE_H
#define BASYCO_LOGICSTATEMACHINE_H


#include <p2p/log/Logger.h>
#include <p2p/logic/LogicManager.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/LogicContext.h>
#include <p2p/logic/sources/LogicStateSource.h>
#include "StateMachine.h"

template<typename Object, typename stateIdType>
class LogicStateMachine : public StateMachine<stateIdType> {
public:
    typedef stateIdType StateIdType;
private:
    Object &object;

public:


    void onEnter(const StateIdType &id) {
        LOGGER("LOGIC MACHINE ON ENTER" + std::to_string(id));
        //@todo handle no logic manager!
        auto &lm = LogicContext::getLogicManagerFromActiveContext();
        auto &source = lm.requireSource<LogicStateSource>();
        source.stateEntered(object, id);
    }

    void onLeave(const StateIdType &id) {
        LOGGER("LOGIC MACHINE ON LEAVE" + std::to_string(id));
        //@todo handle no logic manager!
        auto &lm = LogicContext::getLogicManagerFromActiveContext();
        auto &source = lm.requireSource<LogicStateSource>();
        source.stateLeft(object, id);
    }

//    LogicStateMachine() : StateMachine<StateIdType>(onEnter, onLeave) {};

    explicit LogicStateMachine(Object &object) : StateMachine<StateIdType>(
            std::bind(&LogicStateMachine<Object, StateIdType>::onEnter, this, std::placeholders::_1),
            std::bind(&LogicStateMachine<Object, StateIdType>::onLeave, this, std::placeholders::_1)),
                                                 object(object) {}

};


#endif //BASYCO_LOGICSTATEMACHINE_H
