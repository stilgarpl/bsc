//
// Created by stilgar on 02.12.2019.
//

#ifndef BSC_SIMPLEJOURNALMETADATAFETCHER_H
#define BSC_SIMPLEJOURNALMETADATAFETCHER_H

#include "JournalMetaDataFetcher.h"

namespace bsc {
    class SimpleJournalMetaDataFetcher : public JournalMetaDataFetcher {
    public:
        JournalMetaData makeMetaData() override;

    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar(cereal::base_class<JournalMetaDataFetcher>(this));
        }


        friend class cereal::access;
    };
}
CEREAL_REGISTER_TYPE(bsc::SimpleJournalMetaDataFetcher)
CEREAL_REGISTER_POLYMORPHIC_RELATION(bsc::JournalMetaDataFetcher, bsc::SimpleJournalMetaDataFetcher)


#endif //BSC_SIMPLEJOURNALMETADATAFETCHER_H
