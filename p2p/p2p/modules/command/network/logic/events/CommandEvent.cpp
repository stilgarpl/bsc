//
// Created by stilgar on 19.11.17.
//

#include "CommandEvent.h"

const std::string &CommandEvent::getCommandName() const {
    return commandName;
}

void CommandEvent::setCommandName(const std::string &commandName) {
    CommandEvent::commandName = commandName;
}


const std::vector<std::string> &CommandEvent::getData() const {
    return data;
}

void CommandEvent::setData(const std::vector<std::string> &data) {
    CommandEvent::data = data;
}

