//
// Created by Krzysztof Tulidowicz on 01.07.19.
//

#ifndef BSC_LOGGERCONTEXT_H
#define BSC_LOGGERCONTEXT_H

#include <context/context/AutoContextSetter.h>
#include <functional>
#include <optional>
#include <string>

namespace bsc {
    class LoggerContext : public AutoContextSetter<LoggerContext> {

    private:
        std::function<std::string(void)> instanceFetcher;

    public:
        void setInstanceFetcher(const std::function<std::string(void)>& instanceFetcher);

        std::string getInstance();
        LoggerContext();
    };
}


#endif //BSC_LOGGERCONTEXT_H
