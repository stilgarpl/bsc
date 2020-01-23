//
// Created by stilgar on 21.08.17.
//

#ifndef BSC_LOGGER_H
#define BSC_LOGGER_H

#include <string>
#include <mutex>
#include <filesystem>

namespace fs = std::filesystem;


#define LOGGER(x) bsc::Logger(fs::path(__FILE__).filename().string()).debug(__LINE__, x);
#define ERROR(x) bsc::Logger(fs::path(__FILE__).filename().string()).error(__LINE__, x);
#define SHOW(x) LOGGER(std::string(#x) +"="+std::to_string(x))

namespace bsc {
    class Logger {
    private:

        std::string loggerName;
    public:

        //@todo think about this global locking: Is it necessary?
        static std::mutex& getLock() {
            static std::mutex lock;
            return lock;
        }

        explicit Logger(std::string name);

        void debug(const std::string& txt);

        void debug(int line, const std::string& txt);

        void error(int line, const std::string& txt);

        void error(const std::string& txt);

        void info(const std::string& txt);
    };
}


#endif //BSC_LOGGER_H
