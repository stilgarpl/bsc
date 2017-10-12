//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_ICONFIG_H
#define BASYCO_ICONFIG_H


#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/base_class.hpp>

class IConfig {
private:
    std::string testField = "dupa";
private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar & testField;
    }


    friend class cereal::access;

public:
    virtual ~IConfig() {

    }
};

CEREAL_REGISTER_TYPE(IConfig);

#endif //BASYCO_ICONFIG_H
