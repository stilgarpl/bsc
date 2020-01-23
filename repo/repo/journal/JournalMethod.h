//
// Created by stilgar on 18.10.17.
//

#ifndef BSC_JOURNALMETHOD_H
#define BSC_JOURNALMETHOD_H
namespace bsc {
    enum JournalMethod {
        //@todo add some real numbers here, in case I need to add more stuff
                none,
        added,
        modified,
        moved,
        deleted,
        ignored,
        unignored,
        forgotten,
    };
}

#endif //BSC_JOURNALMETHOD_H

