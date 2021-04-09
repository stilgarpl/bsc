//
// Created by stilgar on 12.10.2020.
//

#ifndef BSC_AUTOCALLER_H
#define BSC_AUTOCALLER_H

#include <functional>
#include <mutex>
namespace bsc {

    template<typename Type, auto fun>
    class AutoCaller {
    private:
        struct Caller {
            Caller(const std::function<void(void)>& f) {
                f();
            }
            // this is intentionally empty
            void init() const {};
        };
        inline const static Caller caller{fun};
        inline const static Type t{};

    public:
        AutoCaller() = delete;
        explicit AutoCaller(int passAnythingHere) {
            // this doesn't do anything, but it forces caller to instantiate
            caller.init();
        }
    };
}// namespace bsc

#endif