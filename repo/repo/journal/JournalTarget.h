//
// Created by stilgar on 06.11.18.
//

#ifndef BSC_JOURNALTARGET_H
#define BSC_JOURNALTARGET_H
namespace bsc {
    enum class JournalTarget {
        none        = 0,
        file        = 1,
        directory   = 2,
        transformer = 3,
    };

}
#endif //BSC_JOURNALTARGET_H
