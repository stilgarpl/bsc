//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_SIGNAL_H
#define BASYCO_SIGNAL_H

#include <memory>
#include <list>
#include <mutex>
#include <functional>
#include "Executor.h"


namespace bsc {
    template<typename... Args>
    class Signal {
    public:
        typedef std::function<void(Args...)> Func;
        typedef std::shared_ptr<Func> FuncPtr;
        typedef std::list<FuncPtr> FuncList;
        //@todo think about removing this mutex:
        mutable std::recursive_mutex signalLock;
    private:
        FuncList funcList;

    public:

        //    virtual int signal(Args... args) {
        //        return signal(Executor::getDefaultExecutor(), args...);
        //    };

        virtual int signal(std::shared_ptr<Executor> executor, Args... args) const {
            std::unique_lock lock(signalLock);
            int executed = 0;
            for (auto&& item : funcList) {
                executor->execute([=]() { (*item)(args...); });
                executed++;
            }
            return executed;
        }

        Signal<Args...>& assign(FuncPtr func) {
            std::unique_lock lock(signalLock);
            funcList.push_back(func);
            return *this;
        }

        Signal<Args...>& assign(const Func& func) {
            std::unique_lock lock(signalLock);
            FuncPtr funcPtr = std::make_shared<Func>(func);
            return assign(funcPtr);
        }

        virtual ~Signal() = default;


    };
}


#endif //BASYCO_SIGNAL_H
