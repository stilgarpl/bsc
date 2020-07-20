//
// Created by Krzysztof Tulidowicz on 29.12.17.
//

#include <ios>
#include <fstream>
#include <utility>
#include "SimplePathRI.h"

bsc::SimplePathRI::SimplePathRI(fs::path resourcePath) : resourcePath(std::move(resourcePath)) {}

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
