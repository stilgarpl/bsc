//
// Created by stilgar on 03.02.19.
//

#ifndef BASYCO_COMMONACTIONS_H
#define BASYCO_COMMONACTIONS_H

#include <p2p/context/Context.h>
#include <p2p/logic/common/ForeachContext.h>


struct CommonActions {

//    every(1s).fireC(foreachAction(broadcast),bulkEval({1,2,3}),bulkEval(getRepoIds()));
    template<typename Func, typename Container>
    static constexpr auto foreachAction(Func func, Container &container) {
        return [container, func](auto &... param) {
            for (const auto &it : container) {
                //@todo replace all that context stuff by ContextStack
//#error "problem z tym, że bulkEval jest uruchomiony przed wejściem tutaj, więc nie może ustawiać po kolei rzeczy"
//#error "więc pomysł jest taki, żeby bulkEval zwrócił referkę do ForeachContext::value, a potem ta petla by zmieniala wartosc tej zmiennej. w razie czego mozna zwracac tez pointer czy cos"
//                auto context = Context::makeContext();
//                context->setParentContext(Context::getActiveContext());
//                auto cur = Context::getActiveContext();
//                Context::setActiveContext(context);
                Context::getActiveContext()->get<ForeachContext<std::decay_t<decltype(it)>>>()->setValue(it);

                func(param...);

//                Context::setActiveContext(Context::getActiveContext()->getParentContext());
            }
        };
    }
};


#endif //BASYCO_COMMONACTIONS_H
