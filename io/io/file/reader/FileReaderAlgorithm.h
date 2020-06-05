//
// Created by stilgar on 11.04.2020.
//

#ifndef BSC_FILEREADERALGORITHM_H
#define BSC_FILEREADERALGORITHM_H

#include "io/translator/PropertiesMetaData.h"
#include <filesystem>

namespace bsc {
    namespace fs = std::filesystem;
    class FileReaderAlgorithm {
    public:
        virtual bool isAlgorithmSuitableForFileType(const MimeFileType&) = 0;
        virtual PropertiesMetaData readMetaData(fs::path)                = 0;
        virtual ~FileReaderAlgorithm()                                   = default;
    };
}// namespace bsc

#endif