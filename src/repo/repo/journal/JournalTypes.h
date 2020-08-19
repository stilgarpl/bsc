//
// Created by Krzysztof Tulidowicz on 18.10.17.
//

#ifndef BSC_JOURNALTYPES_H
#define BSC_JOURNALTYPES_H

#include <chrono>
#include <filesystem>
#include <string>

namespace bsc {
    //typedef std::string ResourceId;
    //@todo why there is a ChecksumId and ChecksumType? what's the difference? If there is none, remove ChecksumId
    using ChecksumId = std::string;

//@todo different Clock maybe? utc_clock?
    typedef std::chrono::system_clock::time_point CommitTimeType;
    typedef std::string ChecksumType;
    //@todo path serialization doesn't work, so it's string
    typedef std::filesystem::path PathType;
}

#endif //BSC_JOURNALTYPES_H
