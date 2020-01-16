//
// Created by stilgar on 19.11.17.
//

#ifndef BASYCO_COMMANDEVENT_H
#define BASYCO_COMMANDEVENT_H

#include <string>
#include <vector>
#include <logic/IEvent.h>


namespace bsc {
    enum class CommandEventId {
        EXECUTE_COMMAND,
        DUMMY_COMMAND,
    };

    class CommandEvent : public bsc::IEvent<CommandEventId> {

    private:
        std::string commandName;
        std::vector<std::string> data; //may be empty, but the number of arguments have to be exactly that what is required for the command

    public:
        //    CommandEvent() = default;

        const std::string& getCommandName() const;

        void setCommandName(const std::string& commandName);

        const std::vector<std::string>& getData() const;

        void setData(const std::vector<std::string>& data);

    };
}


#endif //BASYCO_COMMANDEVENT_H
