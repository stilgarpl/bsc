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
#include <utility>
#include <ostream>


template<typename ConfigType>
inline constexpr const auto getConfigName() {
    return "null";
}

#define CONFIG_NAME(TYPE, NAME)  template<> inline constexpr const auto getConfigName<TYPE>() { return NAME; }


class IConfig {
public:
    typedef std::string IdType;
    const std::string _name = "null";

    IConfig() = default;

    explicit IConfig(std::string _name) : _name(std::move(_name)) {}

private:
    template<class Archive>
    void serialize(Archive &ar) {

    }


    friend class cereal::access;

public:
    virtual ~IConfig() = default;

    IConfig &operator=(const IConfig &other) {

        return *this;
    }

    const std::string &getConfigId() const {
        return _name;
    }
};

CEREAL_REGISTER_TYPE(IConfig)

#endif //BASYCO_ICONFIG_H
