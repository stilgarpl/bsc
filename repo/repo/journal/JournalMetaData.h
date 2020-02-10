//
// Created by stilgar on 21.11.2019.
//

#ifndef BSC_JOURNALMETADATA_H
#define BSC_JOURNALMETADATA_H


#include <core/utils/cereal_include.h>
#include <core/utils/crypto.h>
#include <p2p/core/node/NodeInfo.h>
#include "JournalTypes.h"

namespace bsc {

    class JournalMetaData {
    private:
        bsc::NodeIdType nodeId;
        std::string userId;
        std::string operatingSystem;

    public:
        [[nodiscard]] const bsc::NodeIdType& getNodeId() const;

        [[nodiscard]] const std::string& getUserId() const;

        [[nodiscard]] const std::string& getOperatingSystem() const;

    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar(CEREAL_NVP(nodeId), CEREAL_NVP(userId), CEREAL_NVP(operatingSystem));
        }

    public:
        JournalMetaData() = default;

        JournalMetaData(bsc::NodeIdType nodeId, std::string userId, std::string operatingSystem);

        ChecksumType getChecksum();

    private:

        friend class cereal::access;

        friend class JournalMetaDataFetcher;
    };
}
#endif //BSC_JOURNALMETADATA_H
