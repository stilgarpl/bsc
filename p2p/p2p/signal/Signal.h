//
// Created by stilgar on 21.08.17.
//

#ifndef BASYCO_SIGNAL_H
#define BASYCO_SIGNAL_H

#include <memory>
#include <list>
#include "Executor.h"


template<typename... Args>
class Signal {
public:
    typedef std::function<void(Args...)> Func;
    typedef std::shared_ptr<Func> FuncPtr;
    typedef std::list<FuncPtr> FuncList;

private:
    FuncList funcList;

public:
    //może przeciążona klasa AsyncSignal
    //albo jakiś globalna funkcja - defaultowa by wywoływała od razu, async by robiła wątek
    //a ta funkcja by tylko wywoływała tamtą z odpowiednimi parametrami
    virtual int signal(Args... args) {
        int executed = 0;
        for (auto &&item : funcList) {
            Executor::execute([=]() { (*item)(args...); });
            executed++;
        }
        return executed;
    };

    Signal<Args...> &assign(FuncPtr func) {
        funcList.push_back(func);
        return *this;
    }

    Signal<Args...> &assign(const Func &func) {
        FuncPtr funcPtr = std::make_shared<Func>(func);
        return assign(funcPtr);
    }

//    void assign(Func func) {
//        FuncPtr funcPtr = std::make_shared<Func>(func);
//        assign(funcPtr);
//    }
};


#endif //BASYCO_SIGNAL_H
