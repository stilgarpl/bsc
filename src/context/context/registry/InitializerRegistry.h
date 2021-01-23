//
// Created by Krzysztof Tulidowicz on 14.10.2020.
//

#ifndef BSC_INITIALIZERREGISTRY_H
#define BSC_INITIALIZERREGISTRY_H

#include "Registry.h"
#include <functional>

namespace bsc {
    template<typename T>
    class InitializerRegistry {
        using Initializer = std::function<void(T&)>;
        static auto& initializerRegistry() {
            static Registry<Initializer> registry;
            return registry;
        }

    public:
        static void registerInitializer(Initializer initializer) { initializerRegistry().addEntry(initializer); }

    protected:
        void initialize() {
            static_assert(std::is_base_of_v<InitializerRegistry, T>, "InitializerRegistry uses CRTP");
            initializerRegistry().forEach([this](const Initializer& i) { i(*static_cast<T*>(this)); });
        }
    };
}// namespace bsc

#endif