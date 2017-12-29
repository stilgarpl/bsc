//
// Created by stilgar on 29.12.17.
//

#include <ios>
#include <fstream>
#include "SimplePathRI.h"

//std::experimental::filesystem::path SimplePathRI::getPath() {
//    return resourcePath;
//}

SimplePathRI::SimplePathRI(const fs::path &resourcePath) : resourcePath(resourcePath) {}

std::shared_ptr<std::istream> SimplePathRI::getResourceInputStream() {
    return std::make_shared<std::fstream>(resourcePath, std::ios::in | std::ios::out | std::fstream::binary);
}

std::shared_ptr<std::ostream> SimplePathRI::getResourceOutputStream() {
    return std::make_shared<std::fstream>(resourcePath, std::ios::in | std::ios::out | std::fstream::binary);
}

bool SimplePathRI::exists() {
    return fs::exists(resourcePath);
}
