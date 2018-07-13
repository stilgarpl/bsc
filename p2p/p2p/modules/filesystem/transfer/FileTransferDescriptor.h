//
// Created by stilgar on 22.11.17.
//

#ifndef BASYCO_FILETRANSFERDESCRIPTOR_H
#define BASYCO_FILETRANSFERDESCRIPTOR_H

#include <experimental/filesystem>
#include <p2p/node/NodeInfo.h>

#include <thread>

namespace fs = std::experimental::filesystem;

enum class FileTransferStatus {
    NOT_STARTED,
    STARTED,
    ATTRIBUTES_ACCQUIRED,
    DOWNLOADING,
    FINISHED,
    ERROR,
};

class FileTransferDescriptor {
private:
    FileTransferStatus status = FileTransferStatus::NOT_STARTED;
    fs::path destinationPath;
    NodeIdType sourceNode;
    fs::path sourcePath;
    //@todo size_t may be too small
    size_t fileSize;
    size_t transferredSize;

    //very private, no getters:

    std::unique_ptr<std::thread> thread = nullptr;

protected:

    void startThread(std::function<void(FileTransferDescriptor &)> f) {
        thread = std::make_unique<std::thread>(f, std::ref(*this));
        //f(*this);
    }

public:

    FileTransferStatus getStatus() const;

    const std::experimental::filesystem::path &getDestinationPath() const;

    const NodeIdType &getSourceNode() const;

    const std::experimental::filesystem::path &getSourcePath() const;

    size_t getFileSize() const;

    size_t getTransferredSize() const;

protected:
    friend class FileTransferControl;

    void setStatus(FileTransferStatus status);

    void setDestinationPath(const std::experimental::filesystem::path &destinationPath);

    void setSourceNode(const NodeIdType &sourceNode);

    void setSourcePath(const std::experimental::filesystem::path &sourcePath);

    void setFileSize(size_t fileSize);

    void setTransferredSize(size_t transferredSize);

public:
    virtual ~FileTransferDescriptor();

};


typedef std::shared_ptr<FileTransferDescriptor> FileTransferDescriptorPtr;

#endif //BASYCO_FILETRANSFERDESCRIPTOR_H
