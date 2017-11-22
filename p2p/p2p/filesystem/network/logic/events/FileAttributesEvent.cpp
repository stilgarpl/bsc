//
// Created by stilgar on 12.10.17.
//

#include "FileAttributesEvent.h"

const std::experimental::filesystem::path &FileAttributesEvent::getPath() const {
    return path;
}

void FileAttributesEvent::setPath(const std::experimental::filesystem::path &path) {
    FileAttributesEvent::path = path;
}
