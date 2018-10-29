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
#include <chrono>
#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <sstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/types/string.hpp>
#include <cryptopp/files.h>

#include <filesystem>

namespace fs = std::filesystem;

class JournalStateData {
private:
    JournalMethod method;
    fs::perms permissions;


private:
    PathType path;
    uintmax_t size = 0;
    std::time_t modificationTime;
    ResourceId checksum; //checksum of the file.
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(method), CEREAL_NVP(path), CEREAL_NVP(size), CEREAL_NVP(checksum), CEREAL_NVP(modificationTime),
           CEREAL_NVP(permissions));
    }


    friend class cereal::access;

public:
    JournalStateData(JournalMethod method, PathType path) : method(method), path(std::move(path)) {
        update();
    };

    JournalStateData() = default;

    JournalMethod getMethod() const {
        return method;
    }

    fs::perms getPermissions() const;

    time_t getModificationTime() const {
        return modificationTime;
    }

    void setMethod(JournalMethod method) {
        JournalStateData::method = method;
    }

    const PathType &getPath() const {
        return path;
    }

    void setPath(const PathType &path) {
        JournalStateData::path = path;
        update();
    }

    void update();

    const ResourceId &getChecksum() const {
        return checksum;
    }

    uintmax_t getSize() const {
        return size;
    }
};


class JournalState {
    ResourceId checksum;
    std::list<JournalStateData> dataList;
    std::chrono::system_clock::time_point commitTime;
    std::shared_ptr<JournalState> previousState = nullptr;

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(checksum), CEREAL_NVP(dataList), CEREAL_NVP(commitTime), CEREAL_NVP(previousState));
    }


    friend class cereal::access;

public:
    void add(const JournalStateData &data);

    //@template change it to something else? probably some kind of replay mechanism?
    const std::list<JournalStateData> &getDataList() const;

    void setDataList(const std::list<JournalStateData> &dataList);

    std::string calculateChecksum();

    void commit();

    const std::shared_ptr<JournalState> &getPreviousState() const;

    void setPreviousState(const std::shared_ptr<JournalState> &previousState);

    const std::chrono::system_clock::time_point &getCommitTime() const;

    bool operator==(const JournalState &rhs) const;

    bool operator!=(const JournalState &rhs) const;

    const ResourceId &getChecksum() const;

    JournalState(const JournalState &other) {
        checksum = other.checksum;
        dataList = other.dataList;
        commitTime = other.commitTime;
        previousState = other.previousState;
    }

    JournalState() = default;

};


#endif //BASYCO_JOURNALSTATE_H
