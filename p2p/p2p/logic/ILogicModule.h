//
// Created by stilgar on 12.10.17.
//

#ifndef BASYCO_ILOGICMODULE_H
#define BASYCO_ILOGICMODULE_H


#include <p2p/logic/sources/AutoSource.h>
#include <p2p/node/INodeModule.h>
#include <p2p/node/INode.h>
#include "LogicManager.h"
#include "LogicObject.h"

//@todo #warning move this class to node modules
class ILogicModule : public INodeModule, public LogicObject {

public:

    bool setupLogic() override {
        return LogicObject::setupLogic();
    }

    explicit ILogicModule(INode &node) : INodeModule(node), LogicObject(node.getLogicManager()) {

    }



protected:

    //when(event<CommandEvent>().withId(CommandId::EXECUTE_COMMAND)).fireNewAction(RUN_COMMAND);d




};


#endif //BASYCO_ILOGICMODULE_H
