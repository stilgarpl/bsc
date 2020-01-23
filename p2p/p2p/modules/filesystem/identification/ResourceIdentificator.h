//
// Created by stilgar on 29.12.17.
//

#ifndef BSC_RESOURCEIDENTIFICATOR_H
#define BSC_RESOURCEIDENTIFICATOR_H

#include <filesystem>
#include <cereal/access.hpp>
#include <core/utils/cereal_include.h>

namespace bsc {
    namespace fs = std::filesystem;

    class ResourceIdentificator {
    public:

        /**
         * returns input stream of the resource
         * @return
         */
        virtual std::shared_ptr<std::istream> getResourceInputStream() = 0;

        /**
         * returns output stream of the resource
         * @return
         */
        virtual std::shared_ptr<std::ostream> getResourceOutputStream() = 0;

        virtual uintmax_t getResourceSize() = 0;

        /**
         * returns true if the resource with this identificator exists on this node.
         * @return
         */
        virtual bool exists() = 0;

    private:
        template<class Archive>
        void serialize(Archive& ar) {
        }


        friend class cereal::access;

    public:
        virtual ~ResourceIdentificator() = default;
    };

    typedef std::shared_ptr<bsc::ResourceIdentificator> ResourceIdentificatorPtr;
}


CEREAL_REGISTER_TYPE(bsc::ResourceIdentificator)


#endif //BSC_RESOURCEIDENTIFICATOR_H
