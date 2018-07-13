//
// Created by stilgar on 11.10.17.
//

#include <p2p/modules/nodeNetworkModule/protocol/context/ConnectionContext.h>
#include <p2p/modules/filesystem/network/logic/sources/FileSource.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/LogicContext.h>
#include <p2p/modules/nodeNetworkModule/protocol/context/ProcessorContext.h>
#include "SendFile.h"

void SendFile::Request::process(Context::Ptr context) {
    BasePacket::process(context);
    auto lc = context->get<LogicContext>();
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

size_t SendFile::Request::getBegin() const {
    return begin;
}

void SendFile::Request::setBegin(size_t begin) {
    Request::begin = begin;
}

size_t SendFile::Request::getEnd() const {
    return end;
}

void SendFile::Request::setEnd(size_t end) {
    Request::end = end;
}

void SendFile::Response::process(Context::Ptr context) {
    BasePacket::process(context);
    auto lc = context->get<LogicContext>();
    if (lc != nullptr) {
        auto fileSource = lc->getLogicManager().getSource<FileSource>();
        auto processorContext = context->get<ProcessorContext>();
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
    std::vector<char> fileContents;
    if (end == 0) {
        fileContents.assign((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
    } else {
//        file.seekg(end);
//        auto endPos = file.tellg();
        file.seekg(begin);
        char rawData[end - begin];
        file.read(rawData, end - begin);
        fileContents.assign(rawData, rawData + sizeof rawData / sizeof rawData[0]);

    }
    data = std::move(fileContents);
}

void SendFile::Response::save_file() const {
    if (!fs::exists(filePath)) {
        std::fstream f(filePath, std::ios_base::out | std::ofstream::binary);
        f.close(); //file now exists always
    }
    std::ofstream file(filePath, std::ios::in | std::ios::out | std::ofstream::binary);
    if (end == 0) {
        std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(file));
    } else {
        file.seekp(begin);
        std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(file));
    }
}

const std::string &SendFile::Response::getFilePath() const {
    return filePath;
}

void SendFile::Response::setFilePath(const std::string &filePath) {
    Response::filePath = filePath;
}

size_t SendFile::Response::getBegin() const {
    return begin;
}

void SendFile::Response::setBegin(size_t begin) {
    Response::begin = begin;
}

size_t SendFile::Response::getEnd() const {
    return end;
}

void SendFile::Response::setEnd(size_t end) {
    Response::end = end;
}
