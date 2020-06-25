//
// Created by stilgar on 25.06.2020.
//

#ifndef BSC_FAKEMETADATAFETCHER_H
#define BSC_FAKEMETADATAFETCHER_H
#include <repo/journal/SimpleJournalMetaDataFetcher.h>
using namespace bsc;
class FakeMetaDataFetcher : public JournalMetaDataFetcher {
    JournalMetaData metaData;

public:
    JournalMetaData makeMetaData() override { return metaData; }

    FakeMetaDataFetcher(std::string nodeId, std::string userId, std::string operatingSystemId)
        : metaData(nodeId, userId, operatingSystemId) {}
};

#endif// BSC_FAKEMETADATAFETCHER_H
