//
// Created by Krzysztof Tulidowicz on 07.11.17.
//

#ifndef BSC_DEPENDECYMANAGED_H
#define BSC_DEPENDECYMANAGED_H

//
//#include "DependencyManager.h"
#include "IDependency.h"
#include "IDependencyManaged.h"
#include "DependencyManager.h"
#include "Dependency.h"

#include <memory>


namespace bsc {
    template<typename T>
    class DependencyManaged : public virtual IDependencyManaged {
    private:
        std::shared_ptr<IDependency> required;
    public:

        virtual const std::shared_ptr<IDependency>& getRequiredDependencies() const {
            return required;
        }

        virtual IDependency::TypeIdType getDependencyId() {
            return DependencyManager::getClassId<T>();
        }

        void setRequired(const std::shared_ptr<IDependency>& required) {
            DependencyManaged::required = required;


            //clean required - remove duplicates and references to itself
            //@todo duplicate clean
            //remove self references
            auto&& ids = required->getDependencyIdents();

            if (std::find_if(ids.begin(), ids.end(),
                             [&, this](auto i) -> bool { return i == this->getDependencyId(); }) !=
                ids.end()) {
                ids.erase(std::remove_if(ids.begin(), ids.end(),
                                         [&, this](auto i) -> bool { return i == this->getDependencyId(); }));
                DependencyManaged::required = std::make_shared<bsc::DependencyPack>(ids);
            }
        }


    protected:

        template<typename ... Args>
        void setRequired() {
            //@todo remove duplicates
            if constexpr (sizeof... (Args) >= 1) {
                required = std::make_shared<bsc::Dependency<Args...>>();
            }
        }

    protected:
        template<typename ... Args>
        void addRequiredDependency() {

            if (required == nullptr) {
                setRequired<Args...>();
                return;
            }

            auto ids = bsc::Dependency<Args...>::getDependencyIds();

            for (auto&& ident : required->getDependencyIdents()) {
                ids.erase(std::remove_if(ids.begin(), ids.end(), [&](auto i) -> bool { return (i == ident); }),
                          ids.end());
            }

            //        logger.debug("before merge");
            //        for (auto &&r : required->getDependencyIdents()) {
            //            logger.debug("r =" + std::to_string(r));
            //        }

            if (ids.size() > 0) {
                auto depPack = std::make_shared<bsc::DependencyPack>();
                depPack->addDependency(required->getDependencyIdents(), ids);
                //clean ids - remove duplicates and references to itself
                //@todo duplicate clean

                setRequired(depPack);
            } else {
                //            logger.debug("NO DEPS");
                // nothing changes.
            }

            //        logger.debug("after merge");
            //        for (auto &&r : required->getDependencyIdents()) {
            //            logger.debug("r =" +std::to_string(r));
            //        }
        }
    };
}


#endif //BSC_DEPENDECYMANAGED_H
