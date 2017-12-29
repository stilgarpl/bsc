//
// Created by stilgar on 29.12.17.
//

#ifndef BASYCO_RESOURCEIDENTIFICATOR_H
#define BASYCO_RESOURCEIDENTIFICATOR_H

#include <experimental/filesystem>
#include <cereal/access.hpp>
#include <p2p/utils/cereal_include.h>

namespace fs = std::experimental::filesystem;

class ResourceIdentificator {
public:

    virtual std::shared_ptr<std::istream> getResourceInputStream() =0;

    virtual std::shared_ptr<std::ostream> getResourceOutputStream() =0;

private:
    template<class Archive>
    void serialize(Archive &ar) {
    }


    friend class cereal::access;
};


CEREAL_REGISTER_TYPE(ResourceIdentificator);

#endif //BASYCO_RESOURCEIDENTIFICATOR_H
