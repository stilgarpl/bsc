//
// Created by Krzysztof Tulidowicz on 09.02.2021.
//

#ifndef BSC_STANDARDFILELISTFETCHERCONSTRAINTS_H
#define BSC_STANDARDFILELISTFETCHERCONSTRAINTS_H

#include "FileListFetcher.h"
namespace bsc {
    namespace StandardFileListFetcherConstraints {

        template<typename T>
        using Matcher = std::function<bool(const std::remove_cvref_t<T>&)>;

        template<typename T>
        Matcher<T> is(T t) {
            return [t](const T& v) { return t == v; };
        }

        template<typename T>
        Matcher<T> before(T t) {
            return [t](const T& v) { return v < t; };
        }

        template<typename T>
        Matcher<T> after(T t) {
            return [t](const T& v) { return v > t; };
        }

        FileListFetcher::Constraint path(const Matcher<fs::path>& matcher);
        FileListFetcher::Constraint filename(const Matcher<std::string>& matcher);
        FileListFetcher::Constraint extension(const Matcher<std::string>& matcher);
        FileListFetcher::Constraint date(const Matcher<std::chrono::file_clock::time_point>& matcher);

    }// namespace StandardFileListFetcherConstraints
}// namespace bsc

#endif