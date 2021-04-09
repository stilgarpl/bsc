//
// Created by Krzysztof Tulidowicz on 01.07.19.
//

#ifndef BSC_LOGGERCONTEXT_H
#define BSC_LOGGERCONTEXT_H

#include <context/context/AutoContextSetter.h>
#include <functional>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>

namespace bsc {
    class LoggerContext : public AutoContextSetter<LoggerContext> {
        //@todo add an ability to set defaults for LoggerContext constructor

    private:
        std::function<std::string(void)> instanceFetcher;
        std::shared_ptr<spdlog::logger> spdlogLogger;

    public:
        void setInstanceFetcher(const std::function<std::string(void)>& instanceFetcher);

        std::string getInstance();
        bool hasInstance();
        friend class Logger;
        LoggerContext();
    };
}


#endif //BSC_LOGGERCONTEXT_H
