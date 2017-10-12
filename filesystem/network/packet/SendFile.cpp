//
// Created by stilgar on 11.10.17.
//

#include <network/protocol/context/ConnectionContext.h>
#include <filesystem/network/logic/sources/FileSource.h>
#include <network/protocol/context/LogicContext.h>
#include <network/protocol/context/ProcessorContext.h>
#include "SendFile.h"

void SendFile::Request::process(Context &context) {
    BasePacket::process(context);
    auto lc = context.get<LogicContext>();
    if (lc != nullptr) {
        auto fileSource = lc->getLogicManager().getSource<FileSource>();
        fileSource->requestFile(this);
    }
}

const std::string &SendFile::Request::getFilePath() const {
    return filePath;
}

void SendFile::Request::setFilePath(const std::string &filePath) {
    Request::filePath = filePath;
}

void SendFile::Response::process(Context &context) {
    BasePacket::process(context);
    auto lc = context.get<LogicContext>();
    if (lc != nullptr) {
        auto fileSource = lc->getLogicManager().getSource<FileSource>();
        auto processorContext = context.get<ProcessorContext>();
        fileSource->fileReceived(std::static_pointer_cast<Response>(processorContext->getThisPacket()));
    }
}


const std::vector<char> &SendFile::Response::getData() const {
    return data;
}

void SendFile::Response::setData(const std::vector<char> &data) {
    Response::data = data;
}

void SendFile::Response::load_file(fs::path path) {
    filePath = path;
    std::ifstream file(path, std::ios::in | std::ios::binary);
    std::vector<char> fileContents((std::istreambuf_iterator<char>(file)),
                                   std::istreambuf_iterator<char>());
    data = std::move(fileContents);
}

void SendFile::Response::save_file() const {
    std::ofstream file(filePath, std::ios::out | std::ofstream::binary);
    std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(file));
}

const std::string &SendFile::Response::getFilePath() const {
    return filePath;
}

void SendFile::Response::setFilePath(const std::string &filePath) {
    Response::filePath = filePath;
}
