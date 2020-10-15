//
// Created by Krzysztof Tulidowicz on 16.04.2020.
//

#include <core/log/Logger.h>
#include <exiv2/exiv2.hpp>
#include <parser/parser/explode.h>
#include <regex>

#include "ImageFileReader.h"
namespace bsc {
    bool ImageFileReader::isAlgorithmSuitableForFileType(const bsc::MimeFileType& type) {
        return type.typeGroup == "image";
    }
    PropertiesMetaData ImageFileReader::readMetaData(fs::path path) {
        PropertiesMetaData result = PropertiesMetaData::object();
        try {
            auto image = Exiv2::ImageFactory::open(path);
            assert(image.get() != nullptr);

            image->readMetadata();

            for (const auto& exifElement : image->exifData()) {
                if (exifElement.key() ==
                    "Exif.Image.DateTime") {//@todo maybe look for others exif elements if datetime is not present?
                    const auto& dateTime     = exifElement.value().toString();
                    result["image.datetime"] = dateTime;
                    std::regex dateTimeRegex("(....):(..):(..) (..):(..):(..)");
                    std::smatch dateTimeMatch;
                    if (std::regex_match(dateTime, dateTimeMatch, dateTimeRegex) &&
                        dateTimeMatch.size() == dateTimeRegex.mark_count() + 1) {
                        result["image"]["date"]["year"]    = dateTimeMatch[1];
                        result["image"]["date"]["month"]   = dateTimeMatch[2];
                        result["image"]["date"]["day"]     = dateTimeMatch[3];
                        result["date"]                     = result["image"]["date"];
                        result["image"]["time"]["hours"]   = dateTimeMatch[4];
                        result["image"]["time"]["minutes"] = dateTimeMatch[5];
                        result["image"]["time"]["seconds"] = dateTimeMatch[6];
                        result["time"]                     = result["image"]["time"];
                        break;// all fields set, stop processing EXIF elements
                    }
                }
            }
        } catch (const std::exception& e) {
            //@todo better error handling
            using namespace std::string_literals;
            ERROR("Image reader exception: "s + e.what());
        }
        return result;
    }
}// namespace bsc