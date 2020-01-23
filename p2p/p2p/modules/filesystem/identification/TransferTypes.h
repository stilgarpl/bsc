//
// Created by stilgar on 30.12.17.
//

#ifndef BSC_TRANSFERID_H
#define BSC_TRANSFERID_H

#include <vector>
#include <cstdint>

namespace bsc {

    typedef unsigned long TransferId;

    typedef uintmax_t TransferSize;
    typedef std::vector<char> RawDataType;


//@todo think about one thing: this enum serves two purposes - as transfer status indicator and as event id type - maybe it should be separated into two enums? or not
    enum class TransferStatus {
        REQUESTED, //transfer start has been requested
        STARTED, //transfer was started, data transfer can begin
        SENDING, //data is being sent
        GET_PROPERTIES, //properties requested
        FINISHING, //data transfer complete, transfer finish requested
        FINISHED, //transfer has finished
        ERROR, //error occured
        QUEUED, //transfer queued
    };

}
#endif //BSC_TRANSFERID_H
