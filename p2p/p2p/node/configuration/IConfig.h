//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_ICONFIG_H
#define BASYCO_ICONFIG_H


#include <core/utils/cereal_include.h>
#include <utility>
#include <ostream>


//template<typename ConfigType>
//inline constexpr const auto getConfigName() {
//    return "null";
//}

//#define CONFIG_NAME(TYPE, NAME)  template<> inline constexpr const auto getConfigName<TYPE>() { return NAME; }


namespace bsc {
    class IConfig {


    private:
        template<class Archive>
        void serialize(Archive& ar) {

        }


        friend class cereal::access;

    public:

    };
}

//CEREAL_REGISTER_TYPE(bsc::IConfig)

#endif //BASYCO_ICONFIG_H
