//
// Created by stilgar on 16.11.17.
//

#include <logic/context/LogicContext.h>
#include "CommandPacket.h"


const std::string& bsc::CommandPacket::Request::getCommandName() const {
    return commandName;
}

void bsc::CommandPacket::Request::setCommandName(const std::string& commandName) {
    Request::commandName = commandName;
}

const std::vector<std::string>& bsc::CommandPacket::Request::getData() const {
    return data;
}

void bsc::CommandPacket::Request::setData(const std::vector<std::string>& data) {
    Request::data = data;
}


bool bsc::CommandPacket::Response::isRunStatus() const {
    return runStatus;
}

void bsc::CommandPacket::Response::setRunStatus(bool runStatus) {
    Response::runStatus = runStatus;
}

const std::string& bsc::CommandPacket::Response::getOutput() const {
    return output;
}

void bsc::CommandPacket::Response::setOutput(const std::string& output) {
    Response::output = output;
}
