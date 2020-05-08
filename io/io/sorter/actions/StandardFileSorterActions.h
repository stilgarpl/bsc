//
// Created by stilgar on 20.04.2020.
//

#ifndef BSC_STANDARDFILESORTERACTIONS_H
#define BSC_STANDARDFILESORTERACTIONS_H
#include <io/sorter/FileSorter.h>

namespace bsc {
    struct StandardFileSorterActions {
        static FileSorter::SortAction copy;
        static FileSorter::SortAction move;
        static FileSorter::SortAction erase;
        static FileSorter::SortAction pretend;
    };
}// namespace bsc

#endif