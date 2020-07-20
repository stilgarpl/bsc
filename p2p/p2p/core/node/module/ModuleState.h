//
// Created by Krzysztof Tulidowicz on 27.10.18.
//

#ifndef BSC_MODULESTATE_H
#define BSC_MODULESTATE_H


namespace bsc {
    enum class ModuleState {
        UNINITIALIZED,
        INITIALIZED,
        LOGIC_READY,
        SUBMODULES_PREPARED,
        READY,
        SHUTDOWN,
    };
}


#endif //BSC_MODULESTATE_H
