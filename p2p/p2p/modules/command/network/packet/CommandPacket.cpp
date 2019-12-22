//
// Created by stilgar on 16.11.17.
//

#include <logic/context/LogicContext.h>
#include "CommandPacket.h"

void CommandPacket::Request::process(Context::Ptr context) {
    //@todo remove obsolete.
//    BasePacket::process(context);
//    LOGGER("processing command request")
//    auto& lc = context->get<LogicContext>();
//        auto commnandSource = lc.getLogicManager().getSource<CommandSource>();
//        commnandSource->commandReceived(this->getId(), this->getCommandName(), this->getData());

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
//@todo remove obsolete.
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
