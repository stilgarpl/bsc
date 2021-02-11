//
// Created by Krzysztof Tulidowicz on 09.02.2021.
//

#include "StandardFileListFetcherConstraints.h"

namespace bsc::StandardFileListFetcherConstraints {

    FileListFetcher::Constraint path(const Matcher<fs::path>& matcher) {
        return [matcher](const fs::path& p) { return matcher(p); };
    }
    FileListFetcher::Constraint filename(const Matcher<std::string>& matcher) {
        return [matcher](const fs::path& p) { return matcher(p.filename().string()); };
    }
    FileListFetcher::Constraint extension(const Matcher<std::string>& matcher) {
        return [matcher](const fs::path& p) { return matcher(p.extension().string()); };
    }
    FileListFetcher::Constraint date(const Matcher<std::chrono::file_clock::time_point>& matcher) {
        return [matcher](const fs::path& p) {
            //@todo handle file does not exist?
            return matcher(fs::last_write_time(p));
        };
    }
}// namespace bsc::StandardFileListFetcherConstraints
