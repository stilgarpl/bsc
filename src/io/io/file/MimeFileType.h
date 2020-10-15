//
// Created by Krzysztof Tulidowicz on 09.04.2020.
//

#ifndef BSC_MIMEFILETYPE_H
#define BSC_MIMEFILETYPE_H

#include <core/factory/AutoFactory.h>
#include <core/factory/Factory.h>
#include <stdexcept>
#include <string>

namespace bsc {
    struct MimeFileType {
        const std::string typeGroup;
        const std::string type;

    private:
        MimeFileType(std::string typeGroup, std::string type);
        friend class MimeFileTypeFactory;

    public:
        MimeFileType(const MimeFileType& other)     = default;
        MimeFileType(MimeFileType&& other) noexcept = default;
    };

    class FileTypeParseException : public std::domain_error {
    public:
        explicit FileTypeParseException(const std::string& arg);
    };

    template<>
    struct FactoryTraits<MimeFileType, NoFactorySpecialization> {
        using SelectorType = std::string;
    };

    class MimeFileTypeFactory : public AutoFactory<MimeFileTypeFactory, MimeFileType> {
    public:
        [[nodiscard]] ProducedObjectType create(const SelectorType& selector) const override;
    };
}// namespace bsc

#endif