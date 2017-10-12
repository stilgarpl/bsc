//
// Created by stilgar on 11.10.17.
//

#include "FileRequestEvent.h"

const std::experimental::filesystem::path &FileRequestEvent::getFilePath() const {
    return filePath;
}

void FileRequestEvent::setFilePath(const std::experimental::filesystem::path &filePath) {
    FileRequestEvent::filePath = filePath;
}

int FileRequestEvent::getBegin() const {
    return begin;
}

void FileRequestEvent::setBegin(int begin) {
    FileRequestEvent::begin = begin;
}

int FileRequestEvent::getEnd() const {
    return end;
}

void FileRequestEvent::setEnd(int end) {
    FileRequestEvent::end = end;
}

Connection *FileRequestEvent::getConnection() const {
    return connection;
}

void FileRequestEvent::setConnection(Connection *connection) {
    FileRequestEvent::connection = connection;
}

BasePacket::IdType FileRequestEvent::getRequestId() const {
    return requestId;
}

void FileRequestEvent::setRequestId(BasePacket::IdType requestId) {
    FileRequestEvent::requestId = requestId;
}
