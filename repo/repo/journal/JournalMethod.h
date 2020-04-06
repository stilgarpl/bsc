//
// Created by stilgar on 18.10.17.
//

#ifndef BSC_JOURNALMETHOD_H
#define BSC_JOURNALMETHOD_H
namespace bsc {
    enum JournalMethod {
        //@todo add some real numbers here, in case I need to add more stuff
        none=0,
        add=1,
        modify=2,
        move=3,
        remove=4,
        ignore=5,
        unignore=6,
        forget=7,
    };
}

#endif//BSC_JOURNALMETHOD_H
