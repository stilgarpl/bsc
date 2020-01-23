//
// Created by stilgar on 23.08.17.
//

#ifndef BSC_SOURCE_H
#define BSC_SOURCE_H


#include <core/thread/Runnable.h>
#include "SourceManager.h"


namespace bsc {

    class ISource : public bsc::Runnable {
    protected:
        SourceManager& sourceManager;
    public:
        explicit ISource(SourceManager& sourceManager);

    protected:


        template<typename T, typename... Args>
        void event(const T& event, Args... args) {
            sourceManager.event(event, args...);
        }

    public:

        void setContext(Context& context) {};

        ~ISource() override = default;

    };
}


#endif //BSC_SOURCE_H
