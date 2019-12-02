//
// Created by stilgar on 02.12.2019.
//

#ifndef BASYCO_JOURNALMETADATAFETCHER_H
#define BASYCO_JOURNALMETADATAFETCHER_H


#include "JournalMetaData.h"
#include <core/utils/cereal_include.h>

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


#endif //BASYCO_JOURNALMETADATAFETCHER_H
