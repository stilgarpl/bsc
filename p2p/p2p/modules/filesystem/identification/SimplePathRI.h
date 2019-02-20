//
// Created by stilgar on 29.12.17.
//

#ifndef BASYCO_SIMPLEPATHRI_H
#define BASYCO_SIMPLEPATHRI_H

#include "p2p/utils/cereal_include.h"
#include "ResourceIdentificator.h"

class SimplePathRI : public ResourceIdentificator {

    //@todo c++17 implemented -> change it to path
    const std::string resourcePath;

public:
    SimplePathRI(const fs::path &resourcePath);

public:
//    std::filesystem::path getPath() override;

private:
public:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(cereal::base_class<ResourceIdentificator>(this), resourcePath);
    }

    //@todo if this gives problems, just remove it and add default constructor
    template<class Archive>
    static void load_and_construct(Archive &ar, cereal::construct<SimplePathRI> &construct) {
        std::string path;
        ar(path);
        construct(path);
    }

    bool exists() override;

    friend class cereal::access;

public:
    std::shared_ptr<std::istream> getResourceInputStream() override;

    std::shared_ptr<std::ostream> getResourceOutputStream() override;

    uintmax_t getResourceSize() override;

};

CEREAL_REGISTER_TYPE(SimplePathRI)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ResourceIdentificator, SimplePathRI)

#endif //BASYCO_SIMPLEPATHRI_H
