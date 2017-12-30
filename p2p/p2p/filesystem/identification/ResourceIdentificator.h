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

    /**
     * returns input stream of the resource
     * @return
     */
    virtual std::shared_ptr<std::istream> getResourceInputStream() =0;

    /**
     * returns output stream of the resource
     * @return
     */
    virtual std::shared_ptr<std::ostream> getResourceOutputStream() =0;

    /**
     * returns true if the resource with this identificator exists on this node.
     * @return
     */
    virtual bool exists() =0;

private:
    template<class Archive>
    void serialize(Archive &ar) {
    }


    friend class cereal::access;
};


CEREAL_REGISTER_TYPE(ResourceIdentificator);


typedef std::shared_ptr<ResourceIdentificator> ResourceIdentificatorPtr;

#endif //BASYCO_RESOURCEIDENTIFICATOR_H
