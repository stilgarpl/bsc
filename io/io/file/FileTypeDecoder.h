//
// Created by stilgar on 09.04.2020.
//

#ifndef BSC_FILETYPEDECODER_H
#define BSC_FILETYPEDECODER_H

#include "FileType.h"
#include <filesystem>

namespace bsc {
    namespace fs = std::filesystem;

    class FileTypeDecoder {

    public:
        FileType getTypeForFile(const fs::path &path);
    };
}// namespace bsc

#endif