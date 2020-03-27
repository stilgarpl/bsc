//
// Created by stilgar on 17.10.17.
//

#ifndef BSC_JOURNALSTATE_H
#define BSC_JOURNALSTATE_H

#include "JournalFuncMap.h"
#include "JournalMetaData.h"
#include "JournalStateData.h"
#include "JournalTypes.h"
#include <cereal/access.hpp>
#include <chrono>
#include <core/utils/cereal_include.h>
#include <list>
#include <sstream>
#include <string>

#include <filesystem>
#include <p2p/modules/filesystem/data/FileData.h>
#include <variant>
#include <core/log/Logger.h>

namespace bsc {



    class JournalState {
        ChecksumType checksum;
        std::list<std::variant<JournalStateData<JournalTarget::none>,JournalStateData<JournalTarget::file>,JournalStateData<JournalTarget::directory>,JournalStateData<JournalTarget::feature>>> dataList;
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
        template<JournalTarget target>
        void add(const JournalStateData<target>& data){
            auto same = std::find_if(dataList.begin(), dataList.end(), [&](auto i) {
              //@todo about that method and target... shouldn't this be an error if we have more than one method on one file?
              return std::visit([&](auto& i) {
                    return data == i;
              },i);
            });
            if (same == dataList.end()) {
                dataList.push_back(data);
            } else {
                LOGGER("error: trying to add same data again!" + data.getDestination());
            }

        }

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

        void replay(const JournalFuncMap& funcMap) const {
            for (const auto& it : dataList) {
                std::visit([&funcMap](const auto& i){
                    funcMap.execute(i.getMethod(),i);
                },it);
            }
        }

    };

}// namespace bsc
#endif//BSC_JOURNALSTATE_H
