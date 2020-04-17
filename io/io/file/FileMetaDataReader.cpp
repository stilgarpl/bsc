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
            algorithms.emplace_back(std::make_unique<GenericFileReader>());
            algorithms.emplace_back(std::make_unique<ImageFileReader>());
        }
        FileMetaDataReaderImplementation(const FileMetaDataReaderImplementation&)  = delete;
        FileMetaDataReaderImplementation(const FileMetaDataReaderImplementation&&) = delete;

    public:
        static FileMetaDataReaderImplementation& instance() {
            static FileMetaDataReaderImplementation self;
            return self;
        }
        FileMetaData readMetaData(const FileType& fileType, const fs::path& path) {
            FileMetaData result;
            for (const auto& algorithm : algorithms) {
                if (algorithm->isAlgorithmSuitableForFileType(fileType)) { result += algorithm->readMetaData(path); }
            }
            return result;
        }
    };

    FileMetaData FileMetaDataReader::readMetaData(const fs::path& path) {
        auto& fileReader = FileMetaDataReaderImplementation::instance();
        return fileReader.readMetaData(fileType, path);
    }

    FileMetaDataReader::FileMetaDataReader(FileType type) : fileType(std::move(type)) {}
}// namespace bsc
