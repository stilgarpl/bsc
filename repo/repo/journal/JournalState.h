#include <utility>

#include <utility>

//
// Created by stilgar on 17.10.17.
//

#ifndef BASYCO_JOURNALSTATE_H
#define BASYCO_JOURNALSTATE_H

#include <list>
#include <string>
#include <cereal/access.hpp>
#include "JournalMethod.h"
#include "JournalTypes.h"
#include "JournalTarget.h"
#include <chrono>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <sstream>
#include <core/utils/cereal_include.h>
#include <cryptopp/files.h>

#include <filesystem>
#include <p2p/modules/filesystem/data/FileData.h>
#include <p2p/node/NodeInfo.h>

namespace fs = std::filesystem;

typedef std::chrono::system_clock::time_point CommitTimeType;
typedef std::string ChecksumType;

class JournalStateData {
private:
    JournalMethod method = JournalMethod::NONE;
    JournalTarget target = JournalTarget::NONE;
    fs::perms permissions = fs::perms::none;
    PathType path;
    uintmax_t size = 0;
    fs::file_time_type modificationTime;
    //@todo bool or type? directory, file, socket, link...
    bool directory = false;
    ChecksumType checksum; //checksum of the file.
    //@todo think about processed indicator. maybe it should store a value instead of just being a toggle?
    bool processed = false; //this does not go into the checksum
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(method), CEREAL_NVP(target), CEREAL_NVP(path), CEREAL_NVP(size), CEREAL_NVP(checksum),
           CEREAL_NVP(modificationTime),
           CEREAL_NVP(permissions), CEREAL_NVP(directory));
    }


    friend class cereal::access;

public:
    JournalStateData(JournalMethod method, JournalTarget target, PathType path, FileData fileData) : method(method),
                                                                                                     target(target),
                                                                                                     path(std::move(
                                                                                                             path)) {
        update(std::move(fileData));
    };

    JournalStateData() = default;

    JournalMethod getMethod() const {
        return method;
    }

    fs::perms getPermissions() const;

    fs::file_time_type getModificationTime() const {
        return modificationTime;
    }

    const PathType &getPath() const {
        return path;
    }

    void update(FileData data);

    const ChecksumType &getChecksum() const {
        return checksum;
    }

    uintmax_t getSize() const {
        return size;
    }

    bool isProcessed() const;

    void setProcessed(bool processed);

    bool isDirectory() const;

    JournalTarget getTarget() const;
};

class JournalMetaData {
private:
    NodeIdType nodeId;
    std::string userId;
    std::string operatingSystem;

public:
    const NodeIdType &getNodeId() const;

    const std::string &getUserId() const;

    const std::string &getOperatingSystem() const;

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(nodeId),CEREAL_NVP(userId),CEREAL_NVP(operatingSystem));
    }

public:
    JournalMetaData();

private:

    friend class cereal::access;
};


class JournalState {
    ChecksumType checksum;
    std::list<JournalStateData> dataList;
    //@todo different clock maybe? steady_clock?
    CommitTimeType commitTime;
    JournalMetaData metaData;
    std::shared_ptr<JournalState> previousState = nullptr;

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(checksum), CEREAL_NVP(dataList), CEREAL_NVP(commitTime), CEREAL_NVP(previousState), CEREAL_NVP(metaData));
    }


    friend class cereal::access;

public:
    void add(const JournalStateData &data);

    //@template change it to something else? probably some kind of replay mechanism?
    std::list<JournalStateData> &getDataList();

    void setDataList(const std::list<JournalStateData> &dataList);

    std::string calculateChecksum();

    void commit();

    const std::shared_ptr<JournalState> &getPreviousState() const;

    void setPreviousState(const std::shared_ptr<JournalState> &previousState);

    const CommitTimeType &getCommitTime() const;

    bool operator==(const JournalState &rhs) const;

    bool operator!=(const JournalState &rhs) const;

    const ChecksumType &getChecksum() const;

    JournalState(const JournalState &other) {
        checksum = other.checksum;
        dataList = other.dataList;
        commitTime = other.commitTime;
        metaData = other.metaData;
        previousState = other.previousState;
    }

    JournalState() = default;

    bool isProcessed() const;

    void clearProcessed();

};


#endif //BASYCO_JOURNALSTATE_H
