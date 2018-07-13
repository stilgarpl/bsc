//
// Created by stilgar on 03.01.18.
//

#ifndef BASYCO_STORAGERESOURCEIDENTIFICATOR_H
#define BASYCO_STORAGERESOURCEIDENTIFICATOR_H


#include <p2p/modules/filesystem/identification/ResourceIdentificator.h>
#include <repo/repository/Repository.h>

class StorageResourceIdentificator : public ResourceIdentificator {
private:
    Repository::RepoIdType repositoryId;
    IStorage::ResourceId objectId;
public:
    std::shared_ptr<std::istream> getResourceInputStream() override;

    std::shared_ptr<std::ostream> getResourceOutputStream() override;

    uintmax_t getResourceSize() override;

    bool exists() override;

    StorageResourceIdentificator(const IRepository::RepoIdType &repositoryId, const ResourceId &objectId);

private:
    template<class Archive>
    void serialize(Archive &ar) {
        ar(cereal::base_class<ResourceIdentificator>(this), repositoryId, objectId);
    }

    template<class Archive>
    static void load_and_construct(Archive &ar, cereal::construct<StorageResourceIdentificator> &construct) {
        Repository::RepoIdType repositoryId;
        IStorage::ResourceId objectId;
        ar(repositoryId, objectId);
        construct(repositoryId, objectId);
    }

    friend class cereal::access;
};

CEREAL_REGISTER_TYPE(StorageResourceIdentificator);


#endif //BASYCO_STORAGERESOURCEIDENTIFICATOR_H
