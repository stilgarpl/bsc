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

class JournalStateData {
private:
    JournalMethod method;

    ///@todo path serialization doesn't work, so it's string
public:
    typedef std::string PathType;
private:
    PathType path;
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(method, path);
    }


    friend class cereal::access;

public:
    JournalStateData(JournalMethod method, const PathType &path) : method(method), path(path) {};

    JournalMethod getMethod() const {
        return method;
    }

    void setMethod(JournalMethod method) {
        JournalStateData::method = method;
    }

    const PathType &getPath() const {
        return path;
    }

    void setPath(const PathType &path) {
        JournalStateData::path = path;
    }
};


class JournalState {
    JournalChecksumType checksum;
    std::list<JournalStateData> dataList;
    std::chrono::system_clock::time_point commitTime;

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(checksum, dataList, commitTime);
    }


    friend class cereal::access;

public:
    void add(const JournalStateData &data);

    ///@template change it to something else? probably some kind of replay mechanism?
    const std::list<JournalStateData> &getDataList() const;

    void setDataList(const std::list<JournalStateData> &dataList);

    std::string calculateChecksum();

    void commit();

};


#endif //BASYCO_JOURNALSTATE_H