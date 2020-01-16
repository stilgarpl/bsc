//
// Created by stilgar on 16.07.18.
//

#ifndef BASYCO_ROLEFILTER_H
#define BASYCO_ROLEFILTER_H


#include "PacketFilter.h"

namespace bsc {
    class RoleFilter : public bsc::PacketFilter {
    public:
        bool filter(BasePacketPtr packet) override;

        ~RoleFilter() override = default;
    };
}


#endif //BASYCO_ROLEFILTER_H
