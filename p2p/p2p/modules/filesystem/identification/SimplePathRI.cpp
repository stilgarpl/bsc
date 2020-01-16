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
bsc::SimplePathRI::SimplePathRI(const fs::path& resourcePath) : resourcePath(resourcePath.string()) {}

std::shared_ptr<std::istream> bsc::SimplePathRI::getResourceInputStream() {
    if (exists()) {
        return std::make_shared<std::ifstream>(resourcePath, std::ios::in | std::fstream::binary);
    } else {
        return nullptr;
    }
}

std::shared_ptr<std::ostream> bsc::SimplePathRI::getResourceOutputStream() {
    return std::make_shared<std::ofstream>(resourcePath, std::ios::out | std::fstream::binary);
}

bool bsc::SimplePathRI::exists() {
    return fs::exists(resourcePath);
}

uintmax_t bsc::SimplePathRI::getResourceSize() {
    return fs::file_size(resourcePath);
}
