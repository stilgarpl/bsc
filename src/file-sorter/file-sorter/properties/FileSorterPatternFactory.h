//
// Created by Krzysztof Tulidowicz on 27.09.2021.
//
#ifndef BSC_FILESORTERPATTERNFACTORY_H
#define BSC_FILESORTERPATTERNFACTORY_H

#include "file-sorter/properties/FileSorterMapperProperties.h"
#include "io/sorter/mappers/FileSorterDateMatcher.h"
#include "io/sorter/mappers/FileSorterMimeMatcher.h"
#include "io/sorter/mappers/FileSorterNameMatcher.h"
#include "io/sorter/mappers/FileSorterSizeMatcher.h"
#include <io/sorter/mappers/FileSorterMapperMatcher.h>

namespace bsc {
    class FileSorterPatternFactory {
    private:
        MimeFileTypeFactory mimeFileTypeFactory{};
        Parser parser;

    public:
        FileSorterMapperMatcher createPatternMatcher(const FileSorterMapperProperties& properties) {

            switch (properties.type()) {
                case MapperType::regex:
                    return matchers::fileSorterNameMatcher(properties.match());
                case MapperType::mime:
                    return matchers::fileSorterMimeMatcher(mimeFileTypeFactory.create(properties.match()));
                case MapperType::size:
                    return matchers::fileSorterSizeMatcher(parser.fromString<std::uintmax_t>(properties.match()), properties.mode());
//                case MapperType::date:
// @todo c++20 get date and cast it to file_clock
//                    return matchers::fileSorterDateMatcher(, properties.mode());

                    break;
                default:
                    throw std::domain_error("unsupported");
            }
        }
    };
}// namespace bsc

#endif