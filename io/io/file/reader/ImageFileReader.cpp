//
// Created by stilgar on 16.04.2020.
//

#include <exiv2/exiv2.hpp>
#include <parser/parser/explode.h>

#include "ImageFileReader.h"
namespace bsc {
    bool ImageFileReader::isAlgorithmSuitableForFileType(const bsc::FileType& type) {
        return type.typeGroup == "image";
    }
    FileMetaData ImageFileReader::readMetaData(fs::path path) {
        FileMetaData result;
        auto image = Exiv2::ImageFactory::open(path);
        assert(image.get() != nullptr);

        image->readMetadata();

        for (const auto& exifElement : image->exifData()) {
            if (exifElement.key() == "Exif.Image.DateTime") {
                const auto& dateTime     = exifElement.value().toString();
                result["image.datetime"] = dateTime;
                std::regex dateTimeRegex("(....):(..):(..) (..):(..):(..)");
                std::smatch dateTimeMatch;
                if (std::regex_match(dateTime, dateTimeMatch, dateTimeRegex) &&
                    dateTimeMatch.size() == dateTimeRegex.mark_count() + 1) {
                    result["date.year"] = result["image.date.year"] = dateTimeMatch[1];
                    result["date.month"] = result["image.date.month"] = dateTimeMatch[2];
                    result["date.day"] = result["image.date.day"] = dateTimeMatch[3];
                    result["time.hours"] = result["image.time.hours"] = dateTimeMatch[4];
                    result["time.minutes"] = result["image.time.minutes"] = dateTimeMatch[5];
                    result["time.seconds"] = result["image.time.seconds"] = dateTimeMatch[6];
                    break;
                }
            }
        }

        return result;
    }
}// namespace bsc