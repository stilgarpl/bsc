//
// Created by stilgar on 22.11.17.
//

#include <p2p/filesystem/network/logic/sources/FileSource.h>
#include <p2p/network/protocol/context/LogicContext.h>
#include <p2p/network/protocol/context/NodeContext.h>
#include "SendAttributes.h"


void SendAttributes::Request::process(Context::Ptr context) {
    BasePacket::process(context);
    NODECONTEXTLOGGER("processing attr request for " + this->path);
    auto lc = context->get<LogicContext>();
    if (lc != nullptr) {
        auto fileSource = lc->getLogicManager().getSource<FileSource>();
        fileSource->requestAttributes(this);
    }
}

const std::string &SendAttributes::Request::getPath() const {
    return path;
}

void SendAttributes::Request::setPath(const std::string &path) {
    Request::path = path;
}

void SendAttributes::Response::process(Context::Ptr context) {
    BasePacket::process(context);
}

const std::string &SendAttributes::Response::getPath() const {
    return path;
}

void SendAttributes::Response::setPath(const std::string &path) {
    Response::path = path;
}

size_t SendAttributes::Response::getSize() const {
    return size;
}

void SendAttributes::Response::setSize(size_t size) {
    Response::size = size;
}
