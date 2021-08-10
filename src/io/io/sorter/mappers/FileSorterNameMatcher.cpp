//
// Created by Krzysztof Tulidowicz on 11.05.2020.
//

#include "FileSorterNameMatcher.h"

namespace bsc {

    namespace matchers{
        FileSorterMapperMatcher fileSorterNameMatcher(const std::string& regexPattern) {
            std::regex regex{regexPattern};
            return [regex](const FileInfo& fileInfo) {
              std::smatch match;
              if (std::regex_search(fileInfo.path.filename().string(), regex)) {
                  return MatchPrecision ::perfect;
              } else {
                  return MatchPrecision ::none;
              }
            };
        }
    };

}// namespace bsc
