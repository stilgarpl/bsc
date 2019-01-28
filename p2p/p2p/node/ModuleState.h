//
// Created by stilgar on 27.10.18.
//

#ifndef BASYCO_MODULESTATE_H
#define BASYCO_MODULESTATE_H


enum class ModuleState {
    UNINITIALIZED,
    INITIALIZED,
    LOGIC_READY,
    SUBMODULES_PREPARED,
    READY,
    SHUTDOWN,
};


#endif //BASYCO_MODULESTATE_H
