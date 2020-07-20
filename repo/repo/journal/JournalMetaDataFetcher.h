//
// Created by Krzysztof Tulidowicz on 02.12.2019.
//

#ifndef BSC_JOURNALMETADATAFETCHER_H
#define BSC_JOURNALMETADATAFETCHER_H


#include "JournalMetaData.h"
#include <core/utils/cereal_include.h>

namespace bsc {
    class JournalMetaDataFetcher {
    public:
        virtual JournalMetaData makeMetaData() = 0;

//protected:
//    JournalMetaData createMetaDataObject(const NodeIdType& nodeId, const std::string& userId, const std::string& operatingSystem) {
//        return JournalMetaData(nodeId,userId,operatingSystem);
//    }

    private:
        template<class Archive>
        void serialize(Archive& ar) {
        }


        friend class cereal::access;

    };
}
CEREAL_REGISTER_TYPE(bsc::JournalMetaDataFetcher)

#endif //BSC_JOURNALMETADATAFETCHER_H
