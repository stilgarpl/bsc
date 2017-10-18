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

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(checksum, dataList);
    }


    friend class cereal::access;

public:
    void add(const JournalStateData &data);

    ///@template change it to something else? probably some kind of replay mechanism?
    const std::list<JournalStateData> &getDataList() const;

    void setDataList(const std::list<JournalStateData> &dataList);

};


#endif //BASYCO_JOURNALSTATE_H
