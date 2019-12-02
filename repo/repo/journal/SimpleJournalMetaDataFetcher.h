//
// Created by stilgar on 02.12.2019.
//

#ifndef BASYCO_SIMPLEJOURNALMETADATAFETCHER_H
#define BASYCO_SIMPLEJOURNALMETADATAFETCHER_H

#include "JournalMetaDataFetcher.h"

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

CEREAL_REGISTER_TYPE(SimpleJournalMetaDataFetcher)
CEREAL_REGISTER_POLYMORPHIC_RELATION(JournalMetaDataFetcher, SimpleJournalMetaDataFetcher)


#endif //BASYCO_SIMPLEJOURNALMETADATAFETCHER_H
