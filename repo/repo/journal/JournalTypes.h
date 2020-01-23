//
// Created by stilgar on 18.10.17.
//

#ifndef BSC_JOURNALTYPES_H
#define BSC_JOURNALTYPES_H

#include <string>
#include <chrono>

namespace bsc {
//typedef std::string ResourceId;
    using ChecksumId = std::string;

//@todo different Clock maybe? utc_clock?
    typedef std::chrono::system_clock::time_point CommitTimeType;
    typedef std::string ChecksumType;
//@todo path serialization doesn't work, so it's string
    typedef std::string PathType;
}

#endif //BSC_JOURNALTYPES_H
