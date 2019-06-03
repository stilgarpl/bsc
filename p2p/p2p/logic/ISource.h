//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_SOURCE_H
#define BASYCO_SOURCE_H


#include <p2p/thread/Runnable.h>
#include "p2p/signal/Signal.h"
#include "p2p/context/Context.h"
#include "p2p/uber/Uber.h"
#include "p2p/log/Logger.h"
#include "SourceManager.h"


class ISource : public Runnable {
protected:
    SourceManager &sourceManager;
public:
    explicit ISource(SourceManager &sourceManager);

protected:


    template<typename T, typename... Args>
    void event(const T &event, Args... args) {
        sourceManager.event(event, args...);
    }
public:

    void setContext(Context &context) {};

    ~ISource() override = default;

};


#endif //BASYCO_SOURCE_H
