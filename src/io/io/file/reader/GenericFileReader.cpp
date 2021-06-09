//
// Created by Krzysztof Tulidowicz on 11.04.2020.
//

#include "GenericFileReader.h"
namespace bsc {
    bool bsc::GenericFileReader::isAlgorithmSuitableForFileType(const bsc::MimeFileType& type) { return true; }

    PropertiesMetaData bsc::GenericFileReader::readMetaData(fs::path path) {
        using namespace std::chrono;
        PropertiesMetaData result = PropertiesMetaData::object();
        if (fs::exists(path)) {
            result["file"]["size"]      = std::to_string(fs::file_size(path));
            result["file"]["name"]      = path.filename().string();
            result["file"]["extension"] = path.filename().extension();
            auto modificationTime       = fs::last_write_time(path);
            auto dp                     = floor<days>(modificationTime);
            //@todo workaround for missing clock converters in GCC: (remove when C++20 is complete)
            auto fileClockEpoch   = 6437660400;
            auto secondsInSysTime = floor<seconds>(modificationTime).time_since_epoch().count() + fileClockEpoch;
            auto dpInSysDays      = floor<days>(system_clock::from_time_t(secondsInSysTime));
            //            year_month_day ymd(dp); //@todo uncomment when removing workaround
            year_month_day ymd(dpInSysDays);
            hh_mm_ss time{floor<milliseconds>(modificationTime - dp)};
            result["date"]["year"]  = std::to_string(static_cast<int>(ymd.year()));
            result["date"]["month"] = std::to_string(static_cast<unsigned int>(ymd.month()));
            result["date"]["day"]   = std::to_string(static_cast<unsigned int>(ymd.day()));
            result["file"]["date"]  = result["date"];
            result["time"]["hours"] = result["file"]["time"]["hours"] = std::to_string(time.hours().count());
            result["time"]["minutes"] = result["file"]["time"]["minutes"] = std::to_string(time.minutes().count());
            result["time"]["seconds"] = result["file"]["time"]["seconds"] = std::to_string(time.seconds().count());
        }
        return result;
    }
}// namespace bsc