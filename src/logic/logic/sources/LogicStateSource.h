//
// Created by Krzysztof Tulidowicz on 26.11.18.
//

#ifndef BSC_LOGICSTATESOURCE_H
#define BSC_LOGICSTATESOURCE_H


#include <core/signal/OrderedExecutor.h>
#include <core/signal/OrderedExecutionPolicy.h>
#include "core/signal/ExecutorPolicyTraits.h"
#include <logic/sources/AutoSource.h>
#include <logic/events/LogicStateEvent.h>

namespace bsc {
    BSC_MAP_POLICY_TO_EXECUTOR(OrderedExecutionPolicy, OrderedExecutor)


    class LogicStateSource : public ISource {

    private:
        AutoSource source;

    public:

        explicit LogicStateSource(SourceManager& sourceManager);


        template<typename Object, typename StateIdType>
        void stateEntered(Object& object, const StateIdType& state) {
            LogicStateEvent<Object, StateIdType> logicEvent(object);
            logicEvent.setEventId(state);
            logicEvent.setMethod(bsc::LogicStateMethod::entered);
            source.generateEvent<bsc::LogicStateEvent<Object, StateIdType>>(logicEvent);
        }

        template<typename Object, typename StateIdType>
        void stateLeft(Object& object, const StateIdType& state) {
            bsc::LogicStateEvent<Object, StateIdType> logicEvent(object);
            logicEvent.setEventId(state);
            logicEvent.setMethod(bsc::LogicStateMethod::left);
            source.generateEvent<bsc::LogicStateEvent<Object, StateIdType>>(logicEvent);
        }

        ~LogicStateSource() override;

        void run() override;

        void onStop() override;

        void onStart() override;
    };

}
#endif //BSC_LOGICSTATESOURCE_H
