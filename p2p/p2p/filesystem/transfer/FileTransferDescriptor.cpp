//
// Created by stilgar on 22.11.17.
//

#include "FileTransferDescriptor.h"

FileTransferStatus FileTransferDescriptor::getStatus() const {
    return status;
}

const std::experimental::filesystem::path &FileTransferDescriptor::getDestinationPath() const {
    return destinationPath;
}

const NodeIdType &FileTransferDescriptor::getSourceNode() const {
    return sourceNode;
}

const std::experimental::filesystem::path &FileTransferDescriptor::getSourcePath() const {
    return sourcePath;
}

size_t FileTransferDescriptor::getFileSize() const {
    return fileSize;
}

size_t FileTransferDescriptor::getTransferredSize() const {
    return transferredSize;
}

void FileTransferDescriptor::setStatus(FileTransferStatus status) {
    FileTransferDescriptor::status = status;
}

void FileTransferDescriptor::setDestinationPath(const std::experimental::filesystem::path &destinationPath) {
    FileTransferDescriptor::destinationPath = destinationPath;
}

void FileTransferDescriptor::setSourceNode(const NodeIdType &sourceNode) {
    FileTransferDescriptor::sourceNode = sourceNode;
}

void FileTransferDescriptor::setSourcePath(const std::experimental::filesystem::path &sourcePath) {
    FileTransferDescriptor::sourcePath = sourcePath;
}

void FileTransferDescriptor::setFileSize(size_t fileSize) {
    FileTransferDescriptor::fileSize = fileSize;
}

void FileTransferDescriptor::setTransferredSize(size_t transferredSize) {
    FileTransferDescriptor::transferredSize = transferredSize;
}

FileTransferDescriptor::~FileTransferDescriptor() {

    ///@todo kill the thread or something.
    if (thread != nullptr && thread->joinable()) {
        thread->join();
    }

}

