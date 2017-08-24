//
// Created by stilgar on 23.08.17.
//

#ifndef BASYCO_SOURCEMANAGER_H
#define BASYCO_SOURCEMANAGER_H


#include "ISource.h"
#include <list>
#include <memory>

class SourceManager {
public:
    typedef std::shared_ptr<ISource> SourcePtr;
    typedef std::list<SourcePtr> SourceList;
    typedef ISource *ProviderPtr;
    typedef std::list<ISource *> ProviderList;
private:
    SourceList sources;
    StaticUber<ProviderList> providers;
public:

    void work() {
        ///@todo cos wymyslec, zebyt to nie zżerało 100% cpu
        for (auto &&i : sources) {
            if (i != nullptr) {
                i->work();
            }
        }
    }

    void addSource(SourcePtr source) {
        sources.push_back(source);
    }

    void removeSource(SourcePtr source) {
        sources.remove(source);
    }

protected:

    template<typename EventType>
    ProviderList &getProviders() {
        return providers.get<EventType>();
    }

public:
    template<typename EventType>
    void registerProvider(ProviderPtr p) {
        providers.get<EventType>().push_back(p);
    }


    template<typename EventType>
    void registerTrigger(typename ISource::SignalType<EventType>::Func func) {
        for (auto &&it : getProviders<EventType>()) {
            it->template getSignal<EventType>().assign(func);
        }
    }

    template<typename EventType>
    void registerTrigger(typename EventType::IdType id, typename ISource::SignalType<EventType>::Func func) {
        for (auto &&it : getProviders<EventType>()) {
            it->template getSignal<EventType>(id).assign(func);
        }
    }
};


#endif //BASYCO_SOURCEMANAGER_H
