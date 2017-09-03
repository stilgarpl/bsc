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
    ///@todo mozna zmienic ten type na list jesli wiecej niz jedno source danego typu bedzie potrzebne
    Uber<Type> sourcesByType;
    StaticUber<ProviderList> providers;
    std::shared_ptr<ISource> lastProvider = nullptr;
public:

    void work() {
        ///@todo cos wymyslec, zebyt to nie zżerało 100% cpu
        for (auto &&i : sources) {
            if (i != nullptr) {
                i->work();
            }
        }
    }

    template<typename SourceType, typename... Args>
    void addSource(Args... args) {
        std::shared_ptr<SourceType> sourcePtr = std::make_shared<SourceType>(args...);
        addSource(sourcePtr);
        auto &sbt = sourcesByType.get<std::shared_ptr<SourceType>>();
        sbt = sourcePtr;
    }

    template<typename SourceType>
    std::shared_ptr<SourceType> getSource() {
        return sourcesByType.get<std::shared_ptr<SourceType>>();
    }

    template<typename SourceType>
    void removeSource() {
        std::shared_ptr<SourceType> sourcePtr = sourcesByType.get<std::shared_ptr<SourceType>>();
        removeSource(sourcePtr);
        sourcesByType.get<std::shared_ptr<SourceType>>() = nullptr;
    }

protected:
    void addSource(SourcePtr source);

    void removeSource(SourcePtr source) {
        sources.remove(source);
    }

protected:

    template<typename EventType, typename... Args>
    ProviderList &getProviders() {
        return providers.get<EventType, Args...>();
    }

public:
    template<typename EventType, typename... Args>
    void registerProvider(ProviderPtr p) {
        providers.get<EventType, Args...>().push_back(p);
    }


    template<typename EventType, typename... Args>
    void registerProvider() {
        providers.get<EventType, Args...>().push_back(lastProvider.get());
    }

    template<typename EventType, typename... Args>
    bool hasProvider() {
        return providers.get<EventType, Args...>().size() > 0;
    };

    template<typename EventType, typename... Args>
    void registerTrigger(const typename ISource::SignalType<EventType>::Func &func) {
        for (auto &&it : getProviders<EventType, Args...>()) {
            it->template getSignal<EventType, Args...>().assign(func);
        }
    }

    template<typename EventType, typename... Args>
    void
    registerTrigger(const typename EventType::IdType &id,
                    const typename ISource::SignalType<EventType, Args...>::Func &func) {
        for (auto &&it : getProviders<EventType, Args...>()) {
            it->template getSignal<EventType, Args...>(id).assign(func);
        }
    }
};


#endif //BASYCO_SOURCEMANAGER_H
