//
// Created by Krzysztof Tulidowicz on 14.10.2020.
//

#ifndef BSC_REGISTRY_H
#define BSC_REGISTRY_H

#include <mutex>
#include <vector>
namespace bsc {
    template<typename T>
    class Registry {
        std::mutex lock;
        std::vector<T> registry;

    public:
        void addEntry(T obj) {
            std::lock_guard l(lock);
            registry.push_back(obj);
        }

        template<typename Func>
        void forEach(const Func& f) requires std::regular_invocable<Func, T> {
            std::lock_guard l(lock);
            std::for_each(registry.begin(), registry.end(), f);
        }
    };

}// namespace bsc

#endif