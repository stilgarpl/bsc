//
// Created by stilgar on 16.11.17.
//

#include <logic/context/LogicContext.h>
#include "CommandPacket.h"


const std::string &CommandPacket::Request::getCommandName() const {
    return commandName;
}

void CommandPacket::Request::setCommandName(const std::string &commandName) {
    Request::commandName = commandName;
}

const std::vector<std::string> &CommandPacket::Request::getData() const {
    return data;
}

void CommandPacket::Request::setData(const std::vector<std::string> &data) {
    Request::data = data;
}


bool CommandPacket::Response::isRunStatus() const {
    return runStatus;
}

void CommandPacket::Response::setRunStatus(bool runStatus) {
    Response::runStatus = runStatus;
}

const std::string& CommandPacket::Response::getOutput() const {
    return output;
}

void CommandPacket::Response::setOutput(const std::string& output) {
    Response::output = output;
}
