//
// Created by stilgar on 14.06.2021.
//

#ifndef BSC_PARSERCONFIGURATION_H
#define BSC_PARSERCONFIGURATION_H
#include <string>
namespace bsc {
    struct ParserConfiguration {
       const char csvDelimiter  = ',';
       const char pairDelimiter = '=';
    };

    using StringType = std::string;
}
#endif// BSC_PARSERCONFIGURATION_H
