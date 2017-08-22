//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_ICONFIG_H
#define BASYCO_ICONFIG_H


#include <cereal/access.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/polymorphic.hpp>

class IConfig {
private:
    template<class Archive>
    void serialize(Archive &ar) {
    }


    friend class cereal::access;
};

//CEREAL_REGISTER_TYPE(IConfig);

#endif //BASYCO_ICONFIG_H
