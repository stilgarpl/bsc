//
// Created by Krzysztof Tulidowicz on 01.07.19.
//

#ifndef BSC_LOGGERCONTEXT_H
#define BSC_LOGGERCONTEXT_H


#include <functional>
#include <optional>
#include <string>

namespace bsc {
    class LoggerContext {

    private:
        std::function<std::string(void)> instanceFetcher;

    public:
        LoggerContext(std::function<std::string()> instanceFetcher);

        std::string getInstance();

    };
}


#endif //BSC_LOGGERCONTEXT_H
