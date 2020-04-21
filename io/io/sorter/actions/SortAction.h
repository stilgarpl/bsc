//
// Created by stilgar on 20.04.2020.
//

#ifndef BSC_SORTACTION_H
#define BSC_SORTACTION_H
#include <filesystem>
namespace bsc {
    namespace fs = std::filesystem;
    /**
     * action to perform on file being sorted. move, copy, pretend or something else
     */
    class SortAction {
    public:
        //@todo sort is bad name. call it execute or sth.
        virtual void sort(const fs::path& from, const fs::path& to) = 0;
    };
}// namespace bsc

#endif