//
// Created by stilgar on 16.04.2020.
//

#ifndef BSC_IMAGEFILEREADER_H
#define BSC_IMAGEFILEREADER_H

#include "FileReaderAlgorithm.h"

namespace bsc {
    class ImageFileReader : public FileReaderAlgorithm {
    public:
        bool isAlgorithmSuitableForFileType(const MimeFileType& type) override;
        PropertiesMetaData readMetaData(fs::path path) override;
        virtual ~ImageFileReader() = default;
    };
}// namespace bsc

#endif