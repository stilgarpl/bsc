//
// Created by Krzysztof Tulidowicz on 21.08.17.
//

#ifndef BSC_LOGGER_H
#define BSC_LOGGER_H

#include <experimental/source_location>
#include <filesystem>
#include <mutex>
#include <optional>
#include <string>

namespace fs = std::filesystem;

#define SHOW(x) logger.debug(std::string(#x) + "=" + std::to_string(x));

namespace bsc {

    class Logger {
    public:
        enum class LogLevel {
            debug,
            info,
            error,
        };

        Logger();

        void debug(const std::string& txt,
                   std::experimental::source_location sourceLocation = std::experimental::source_location::current());
        void info(const std::string& txt,
                  std::experimental::source_location sourceLocation = std::experimental::source_location::current());
        void error(const std::string& txt,
                   std::experimental::source_location sourceLocation = std::experimental::source_location::current());

    private:
        void doLog(LogLevel level, const std::string& txt, std::experimental::source_location sourceLocation);
    };

    inline thread_local Logger logger = {};
}


#endif //BSC_LOGGER_H
