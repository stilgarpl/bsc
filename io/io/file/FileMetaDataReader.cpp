//
// Created by stilgar on 11.04.2020.
//

#include "FileMetaDataReader.h"
#include <io/file/reader/GenericFileReader.h>

#include <io/file/reader/ImageFileReader.h>
#include <utility>
namespace bsc {

    class FileMetaDataReaderImplementation {
    private:
        std::vector<std::unique_ptr<FileReaderAlgorithm>> algorithms;
        FileMetaDataReaderImplementation() {
            // algorithms have to be in order from most specific to least specific
            algorithms.emplace_back(std::make_unique<ImageFileReader>());
            algorithms.emplace_back(std::make_unique<GenericFileReader>());
        }
        FileMetaDataReaderImplementation(const FileMetaDataReaderImplementation&)  = delete;
        FileMetaDataReaderImplementation(const FileMetaDataReaderImplementation&&) = delete;

    public:
        static FileMetaDataReaderImplementation& instance() {
            static FileMetaDataReaderImplementation self;
            return self;
        }
        PropertiesMetaData readMetaData(const MimeFileType& fileType, const fs::path& path) {
            PropertiesMetaData result = PropertiesMetaData::object();
            for (const auto& algorithm : algorithms) {
                if (algorithm->isAlgorithmSuitableForFileType(fileType)) {
                    const auto& meta = algorithm->readMetaData(path);
                    result.insert(meta.begin(), meta.end());
                    //                    LOGGER("result = " + result.dump(2))
                }
            }
            return result;
        }
    };

    PropertiesMetaData FileMetaDataReader::readMetaData(const fs::path& path) {
        auto& fileReader = FileMetaDataReaderImplementation::instance();
        return fileReader.readMetaData(fileType, path);
    }

    FileMetaDataReader::FileMetaDataReader(MimeFileType type) : fileType(std::move(type)) {}
}// namespace bsc
