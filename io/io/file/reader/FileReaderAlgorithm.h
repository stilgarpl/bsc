//
// Created by stilgar on 11.04.2020.
//

#ifndef BSC_FILEREADERALGORITHM_H
#define BSC_FILEREADERALGORITHM_H

#include "../FileMetaData.h"
#include "../FileType.h"
#include <filesystem>

namespace bsc {
    namespace fs = std::filesystem;
    class FileReaderAlgorithm {
    public:
        virtual bool isAlgorithmSuitableForFileType(const FileType&) = 0;
        virtual FileMetaData readMetaData(fs::path)                  = 0;
    };
}// namespace bsc

#endif