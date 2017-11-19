//
// Created by stilgar on 19.11.17.
//

#ifndef BASYCO_COMMANDEVENT_H
#define BASYCO_COMMANDEVENT_H

#include <string>
#include <vector>
#include <p2p/logic/IEvent.h>

enum class CommandEventId {
    EXECUTE_COMMAND,
};

class CommandEvent : public IEvent<CommandEventId> {

private:
    std::string commandName;
    std::vector<std::string> modules; //may be empty
    std::vector<std::string> data; //may also be empty, but the number of arguments have to be exactly that what is required for the command

public:
    const std::string &getCommandName() const;

    void setCommandName(const std::string &commandName);

    const std::vector<std::string> &getModules() const;

    void setModules(const std::vector<std::string> &modules);

    const std::vector<std::string> &getData() const;

    void setData(const std::vector<std::string> &data);

};


#endif //BASYCO_COMMANDEVENT_H
