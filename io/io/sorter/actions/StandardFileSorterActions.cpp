//
// Created by stilgar on 20.04.2020.
//

#include "StandardFileSorterActions.h"
namespace bsc {
    void bsc::StandardFileSorterActions::Copy::sort(const std::filesystem::path& from,
                                                    const std::filesystem::path& to) {
        //@todo check for errors? throw ? anything?
        //@todo create directories if not exist?
        fs::create_directories(to.parent_path());
        fs::copy(from, to);
        //@todo copy properties of the file
    }
    void StandardFileSorterActions::Move::sort(const fs::path& from, const fs::path& to) {
        //@todo check for errors? throw ? anything?
        //@todo create directories if not exist?
        fs::create_directories(to.parent_path());
        fs::rename(from, to);
    }
    void StandardFileSorterActions::Pretend::sort(const fs::path& from, const fs::path& to) {
        // do nothing
        //@todo this class could somehow return the list of files that it would process if it would do anything... maybe
        //it could take container by reference in the constructor and fill it?
    }
}// namespace bsc
