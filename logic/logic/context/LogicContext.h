//
// Created by stilgar on 29.08.17.
//

#ifndef BSC_LOGICCONTEXT_H
#define BSC_LOGICCONTEXT_H


#include <logic/LogicManager.h>


namespace bsc {
    class LogicContext {

    private:
        LogicManager& logicManager;

    public:
        LogicContext(LogicManager& logicManager);

        LogicManager& getLogicManager() const;

        //shortcut method for getting logic manager from active context
        static LogicManager& getLogicManagerFromActiveContext() {
            return Context::getActiveContext()->get<LogicContext>().getLogicManager();
        }
    };
}


#endif //BSC_LOGICCONTEXT_H
