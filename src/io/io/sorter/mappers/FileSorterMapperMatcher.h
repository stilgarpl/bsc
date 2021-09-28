//
// Created by Krzysztof Tulidowicz on 20.04.2020.
//

#ifndef BSC_FILESORTERMAPPERMATCHER_H
#define BSC_FILESORTERMAPPERMATCHER_H
#include <filesystem>
#include <functional>
#include <io/file/FileInfo.h>
namespace bsc {
    namespace fs = std::filesystem;

    enum class MatchPrecision {
        none    = 0,
        partial = 16,
        perfect = 64,
    };

    enum class MapperMatcherMode {
        none,
        equal,
        greater,
        greaterEqual,
        less,
        lessEqual,
        notEqual,
    };

    template<typename T>
    class ModeComparator {

    public:
        bool compare(const T& t1, const T& t2, MapperMatcherMode comparison) {
            switch (comparison) {
                default:
                case MapperMatcherMode::none:
                    return true;
                case MapperMatcherMode::equal:
                    return std::equal_to{}(t1, t2);
                case MapperMatcherMode::greater:
                    return std::greater{}(t1, t2);
                case MapperMatcherMode::greaterEqual:
                    return std::greater_equal{}(t1, t2);
                case MapperMatcherMode::less:
                    return std::less{}(t1, t2);
                case MapperMatcherMode::lessEqual:
                    return std::less_equal{}(t1, t2);
                case MapperMatcherMode::notEqual:
                    return std::not_equal_to{}(t1, t2);
            }
        }
    };

    using FileSorterMapperMatcher = std::function<MatchPrecision(const FileInfo& path)>;

}// namespace bsc

#endif