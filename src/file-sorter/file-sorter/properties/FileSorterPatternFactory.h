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
        [[nodiscard]] std::vector<FileSorterMapperMatcher> createPatternMatchers(const FileSorterMapperProperties& properties) const {
            std::vector<FileSorterMapperMatcher> result{};
            for (const auto& matcher : properties.matchers()) {

                switch (matcher.type()) {
                    case MapperType::regex:
                        result.push_back(matchers::fileSorterNameMatcher(matcher.match()));
                        break;
                    case MapperType::mime:
                        result.push_back(matchers::fileSorterMimeMatcher(mimeFileTypeFactory.create(matcher.match())));
                        break;
                    case MapperType::size:
                        result.push_back(
                                matchers::fileSorterSizeMatcher(parser.fromString<std::uintmax_t>(matcher.match()), matcher.mode()));
                        break;
                        //                case MapperType::date:
                        // @todo c++20 get date and cast it to file_clock
                        //                    return matchers::fileSorterDateMatcher(, properties.mode());
//                        break;
                    default:
                        throw std::domain_error("unsupported");
                }
            }
            return result;
        }
    };
}// namespace bsc

#endif