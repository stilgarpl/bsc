//
// Created by stilgar on 20.04.2020.
//

#ifndef BSC_STANDARDFILESORTERACTIONS_H
#define BSC_STANDARDFILESORTERACTIONS_H

#include "SortAction.h"
namespace bsc {
    class StandardFileSorterActions {
    public:
        class Copy : public SortAction {
        public:
            void sort(const fs::path& from, const fs::path& to) override;
        };

        class Move : public SortAction {
        public:
            void sort(const fs::path& from, const fs::path& to) override;
        };

        class Pretend : public SortAction {
        public:
            void sort(const fs::path& from, const fs::path& to) override;
        };
    };
}// namespace bsc

#endif