//
// Created by Krzysztof Tulidowicz on 03.01.18.
//

#ifndef BSC_STORAGERESOURCEIDENTIFICATOR_H
#define BSC_STORAGERESOURCEIDENTIFICATOR_H


#include <p2p/modules/filesystem/identification/ResourceIdentificator.h>

namespace bsc {

    class StorageResourceIdentificator : public bsc::ResourceIdentificator {
    private:
        IStorage::StorageId storageId;
        IStorage::ResourceId objectId;
    public:
        std::shared_ptr<std::istream> getResourceInputStream() override;

        std::shared_ptr<std::ostream> getResourceOutputStream() override;

        uintmax_t getResourceSize() override;

        bool exists() override;

        StorageResourceIdentificator(IStorage::StorageId storageId, IStorage::ResourceId objectId);

    private:
        template<class Archive>
        void serialize(Archive& ar) {
            ar(cereal::base_class<ResourceIdentificator>(this), storageId, objectId);
        }

        template<class Archive>
        static void load_and_construct(Archive& ar, cereal::construct<StorageResourceIdentificator>& construct) {
            IStorage::StorageId storageId;
            IStorage::ResourceId objectId;
            ar(storageId, objectId);
            construct(storageId, objectId);
        }

        friend class cereal::access;
    };
}
CEREAL_REGISTER_TYPE(bsc::StorageResourceIdentificator)


#endif //BSC_STORAGERESOURCEIDENTIFICATOR_H