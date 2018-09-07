//
// Created by stilgar on 12.10.17.
//

#include "FileAttributesEvent.h"

const std::filesystem::path &FileAttributesEvent::getPath() const {
    return path;
}

void FileAttributesEvent::setPath(const std::filesystem::path &path) {
    FileAttributesEvent::path = path;
}

FileAttributesEvent::FileAttributesEvent(BasePacket::IdType requestId) : NetworkingEvent(requestId) {}
