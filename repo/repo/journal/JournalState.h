#include <utility>

#include <utility>

//
// Created by stilgar on 17.10.17.
//

#ifndef BSC_JOURNALSTATE_H
#define BSC_JOURNALSTATE_H

#include "JournalMetaData.h"
#include "JournalMethod.h"
#include "JournalTarget.h"
#include "JournalTypes.h"
#include <cereal/access.hpp>
#include <chrono>
#include <core/utils/cereal_include.h>
#include <list>
#include <sstream>
#include <string>

#include <filesystem>
#include <p2p/modules/filesystem/data/FileData.h>

namespace bsc {
    namespace fs = std::filesystem;

    class JournalStateData {
    private:
        JournalMethod method = JournalMethod::none;
        JournalTarget target = JournalTarget::none;
        PathType path{};
        uint64_t size = 0;
        fs::perms permissions = fs::perms::none;
        //@todo C++20 this should probably be changed to utc_clock. epoch of file_time_type is unspecified and it may cause problems.
        fs::file_time_type modificationTime{};
        ChecksumType resourceChecksum{};//checksum of the file.
        ChecksumType checksum{};        // checksum of the whole structure
        bool processed = false;         //this does not go into the checksum
    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar(CEREAL_NVP(method), CEREAL_NVP(target), CEREAL_NVP(path), CEREAL_NVP(size), CEREAL_NVP(resourceChecksum),
               CEREAL_NVP(modificationTime),
               CEREAL_NVP(permissions));
        }


        friend class cereal::access;

    public:
        JournalStateData(JournalMethod method, JournalTarget target, PathType path, bsc::FileData fileData) : method(
                                                                                                                      method),
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

        const PathType& getPath() const {
            return path;
        }

        void update(bsc::FileData data);

        const ChecksumType& getResourceChecksum() const {
            return resourceChecksum;
        }

        const ChecksumType calculateChecksum() const {
            std::string hash = "";
            std::stringstream ss;
            ss << std::to_string(method) << std::to_string(static_cast<std::underlying_type_t<JournalTarget>>(target))
               << path.string() << std::to_string(size) << std::to_string(static_cast<std::underlying_type_t<fs::perms>>(permissions)) << std::to_string(modificationTime.time_since_epoch().count()) << resourceChecksum;

            hash = bsc::calculateSha1OfString(ss.str());

            return hash;
        }

        uintmax_t getSize() const {
            return size;
        }

        bool isProcessed() const;

        void setProcessed(bool processed);

        bool isDirectory() const;

        JournalTarget getTarget() const;
    };


    class JournalState {
        ChecksumType checksum;
        std::list<JournalStateData> dataList;
        CommitTimeType commitTime;
        JournalMetaData metaData;
        std::shared_ptr<JournalState> previousState = nullptr;

    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar(CEREAL_NVP(checksum), CEREAL_NVP(dataList), CEREAL_NVP(commitTime), CEREAL_NVP(previousState),
               CEREAL_NVP(metaData));
        }


        friend class cereal::access;

    public:
        void add(const JournalStateData& data);

        //@template change it to something else? probably some kind of replay mechanism?
        std::list<JournalStateData>& getDataList();

        void setDataList(const std::list<JournalStateData>& dataList);

        std::string calculateChecksum();

        void commit(CommitTimeType);

        const std::shared_ptr<JournalState>& getPreviousState() const;

        void setPreviousState(const std::shared_ptr<JournalState>& previousState);

        const CommitTimeType& getCommitTime() const;

        bool operator==(const JournalState& rhs) const;

        bool operator!=(const JournalState& rhs) const;

        const ChecksumType& getChecksum() const;

        JournalState(const JournalState& other) {
            checksum = other.checksum;
            dataList = other.dataList;
            commitTime = other.commitTime;
            metaData = other.metaData;
            previousState = other.previousState;
        }

        JournalState() = default;

        JournalState(JournalMetaData metaData);

        bool isProcessed() const;

        void clearProcessed();
    };

}// namespace bsc
#endif//BSC_JOURNALSTATE_H
