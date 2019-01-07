//
// Created by stilgar on 16.11.17.
//

#include <p2p/modules/command/network/logic/sources/CommandSource.h>
#include <p2p/node/context/LogicContext.h>
#include "CommandPacket.h"

void CommandPacket::Request::process(Context::Ptr context) {
    BasePacket::process(context);
    LOGGER("processing command request")
    auto lc = context->get<LogicContext>();
    if (lc != nullptr) {
        auto commnandSource = lc->getLogicManager().getSource<CommandSource>();
        commnandSource->commandReceived(this->getId(), this->modules, this->commandName, this->data);
    }
}

const std::vector<std::string> &CommandPacket::Request::getModules() const {
    return modules;
}

void CommandPacket::Request::setModules(const std::vector<std::string> &modules) {
    Request::modules = modules;
}

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

void CommandPacket::Response::process(Context::Ptr context) {
    BasePacket::process(context);
    LOGGER("command response received")
}

bool CommandPacket::Response::isRunStatus() const {
    return runStatus;
}

void CommandPacket::Response::setRunStatus(bool runStatus) {
    Response::runStatus = runStatus;
}
