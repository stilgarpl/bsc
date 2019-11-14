//
// Created by stilgar on 29.12.17.
//

#include <ios>
#include <fstream>
#include "SimplePathRI.h"

//std::filesystem::path SimplePathRI::getPath() {
//    return resourcePath;
//}
//@todo c++17 implemented -> remove .string()
SimplePathRI::SimplePathRI(const fs::path& resourcePath) : resourcePath(resourcePath.string()) {}

std::shared_ptr<std::istream> SimplePathRI::getResourceInputStream() {
    if (exists()) {
        return std::make_shared<std::ifstream>(resourcePath, std::ios::in | std::fstream::binary);
    } else {
        return nullptr;
    }
}

std::shared_ptr<std::ostream> SimplePathRI::getResourceOutputStream() {
    return std::make_shared<std::ofstream>(resourcePath, std::ios::out | std::fstream::binary);
}

bool SimplePathRI::exists() {
    return fs::exists(resourcePath);
}

uintmax_t SimplePathRI::getResourceSize() {
    return fs::file_size(resourcePath);
}
