//
// Created by stilgar on 18.10.17.
//

#ifndef BASYCO_JOURNALTYPES_H
#define BASYCO_JOURNALTYPES_H

#include <string>
#include <chrono>

//typedef std::string ResourceId;
using ChecksumId = std::string;

//@todo different Clock maybe? utc_clock?
typedef std::chrono::system_clock::time_point CommitTimeType;
typedef std::string ChecksumType;
//@todo path serialization doesn't work, so it's string
typedef std::string PathType;


#endif //BASYCO_JOURNALTYPES_H
