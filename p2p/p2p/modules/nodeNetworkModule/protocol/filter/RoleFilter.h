//
// Created by stilgar on 16.07.18.
//

#ifndef BASYCO_ROLEFILTER_H
#define BASYCO_ROLEFILTER_H


#include "PacketFilter.h"

class RoleFilter : public PacketFilter {
public:
    bool filter(BasePacketPtr packet) override;

};


#endif //BASYCO_ROLEFILTER_H
