//
// Created by Krzysztof Tulidowicz on 09.04.2020.
//

#include "RepositorySpecialIncomingHandler.h"
#include <io/file/FileMetaDataReader.h>
#include <io/file/MimeFileTypeDecoder.h>

bsc::RepositorySpecialIncomingHandler::RepositorySpecialIncomingHandler(const fs::path& incomingPath)
    : incomingPath(incomingPath) {
    handleIncoming(incomingPath);
}

void bsc::RepositorySpecialIncomingHandler::handleIncoming(const fs::path& path) {
    // make sure path exists and it's a directory
    if (!fs::exists(path)) { throw RepositoryIncomingHandlerException("Path does not exist: " + path.string());
    }

    if (!fs::is_directory(path)) {
        throw RepositoryIncomingHandlerException("Path is not a directory: " + path.string());
    }

    for (auto &entry: fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file() && !entry.is_directory()) {
            processIncomingFile(entry.path());
        }
    }

}

void bsc::RepositorySpecialIncomingHandler::processIncomingFile(const fs::path &path) {
    MimeFileTypeDecoder decoder;
    // get the file type category
    auto fileType = decoder.getTypeForFile(path);
    // get the file metadata -- modification time etc
    FileMetaDataReader metaDataReader(fileType);
    // get the destination path
    // move file to destination path
}

bsc::RepositoryIncomingHandlerException::RepositoryIncomingHandlerException(const std::string &arg)
        : domain_error(arg) {}
