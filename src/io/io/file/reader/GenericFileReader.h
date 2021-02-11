//
// Created by Krzysztof Tulidowicz on 11.04.2020.
//

#ifndef BSC_GENERICFILEREADER_H
#define BSC_GENERICFILEREADER_H

#include "FileReaderAlgorithm.h"

namespace bsc {
    class GenericFileReader : public FileReaderAlgorithm {
    private:
        const std::string prefix = "file";

    public:
        bool isAlgorithmSuitableForFileType(const MimeFileType& type) override;
        PropertiesMetaData readMetaData(fs::path path) override;
        virtual ~GenericFileReader() = default;
    };
}// namespace bsc

#endif