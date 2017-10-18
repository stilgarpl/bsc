//
// Created by stilgar on 17.10.17.
//

#include "JournalState.h"

void JournalState::add(const JournalStateData &data) {
    dataList.push_back(data);
}

const std::list<JournalStateData> &JournalState::getDataList() const {
    return dataList;
}

void JournalState::setDataList(const std::list<JournalStateData> &dataList) {
    JournalState::dataList = dataList;
}
