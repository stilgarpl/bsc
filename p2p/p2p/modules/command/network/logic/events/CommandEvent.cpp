//
// Created by stilgar on 19.11.17.
//

#include "CommandEvent.h"

const std::string& bsc::CommandEvent::getCommandName() const {
    return commandName;
}

void bsc::CommandEvent::setCommandName(const std::string& commandName) {
    CommandEvent::commandName = commandName;
}


const std::vector<std::string>& bsc::CommandEvent::getData() const {
    return data;
}

void bsc::CommandEvent::setData(const std::vector<std::string>& data) {
    CommandEvent::data = data;
}

