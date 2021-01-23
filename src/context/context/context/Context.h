//
// Created by Krzysztof Tulidowicz on 20.08.17.
//

#ifndef BSC_CONTEXT_H
#define BSC_CONTEXT_H

#include "context/registry/InitializerRegistry.h"
#include <atomic>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>

namespace bsc {
    class InvalidContextException : public std::invalid_argument {
    public:
        explicit InvalidContextException(const std::string& arg);

        explicit InvalidContextException(const char* string);
    };

    class InvalidContextValueException : public std::invalid_argument {
    public:
        explicit InvalidContextValueException(const std::string& arg);
    };

    class ContextLoopException : public std::domain_error {
    public:
        explicit ContextLoopException(const std::string& arg);
    };

    class Context : public InitializerRegistry<Context> {
    public:
        template<bool owning>
        class ContextPtr {
            std::shared_ptr<Context> ptr = nullptr;

        protected:
            //@todo this constructor is needed for Context::makeContext. try to move creating shared_ptr there.
            explicit ContextPtr(std::shared_ptr<Context> ptr) : ptr(std::move(ptr)) {}

        public:
            Context* operator->() const { return ptr.get(); }

            Context& operator*() const { return *ptr; }
            [[nodiscard]] bool hasValue() const { return ptr != nullptr; }

            bool operator==(const ContextPtr& other) const { return ptr == other.ptr; }

            ContextPtr() = default;
            friend class Context;

            //            @todo this was an attempt to have proper semantics between owning and non owning context ptr - try to implement it
            // owning ptr can only be moved from other owning (like unique_ptr)
            //                         ContextPtr(const ContextPtr<true>&) requires owning = delete;
            ContextPtr(const ContextPtr<false>&) requires owning = delete;
            ContextPtr(ContextPtr<true>&& other) noexcept requires owning { ptr = std::move(other.ptr); }
            ContextPtr(ContextPtr<false>&& other) requires owning = delete;

            // non owning can be copied from anything, but moved only from non owning
            ContextPtr(const ContextPtr<true>& other) requires(!owning) { ptr = other.ptr; }
            ContextPtr(const ContextPtr<false>& other) requires(!owning) { ptr = other.ptr; }
            ContextPtr(ContextPtr<true>&& other) noexcept requires(!owning) = delete;
            ContextPtr(ContextPtr<false>&& other) noexcept requires(!owning) { ptr = std::move(other.ptr); }

            ContextPtr<owning>& operator=(const ContextPtr<owning>& other) requires(!owning) = default;
        };
        typedef ContextPtr<false> Ptr;
        typedef ContextPtr<true> OwnPtr;

        template<typename T>
        class Entry {
        private:
            std::shared_ptr<T> obj;
            //@todo throw exception if obj == null?
            explicit Entry(const std::shared_ptr<T>& obj) : obj(obj) {}

        public:
            operator T&() { return *obj; }

            T* operator->() {
                return obj.get();
            }

            T& operator*() {
                return *obj;
            }

            friend class Context;
        };

    private:
        typedef unsigned int KeyType;
        typedef unsigned int TypeIdType;
        constexpr static KeyType defaultKey = 0;

    private:
        //@todo remove debug id
        std::string debugId;
        bool defaultContext = false;
        mutable std::recursive_mutex contextLock;
        // initialized to nullptr in .cpp file
        thread_local static Context::Ptr activeContext;
        //@todo add thread safety - mutex, lock and locking of the parent before accessing it
        Context::Ptr parentContext;

    public:
        bool isDefaultContext() const;

    private:

        static TypeIdType getNextTypeId() {
            static std::atomic<TypeIdType> val = 0;
            return val++;
        }

        static KeyType getNextKey() {
            static std::atomic<KeyType> val = 1;
            return val++;
        }

        //@todo possible improvement: add key offset for different types, so for example keys "test" and 1 do not point to the same values
        template<typename T>
        KeyType getKey(const T& t) const {
            static std::map<T, KeyType> keyMap;
            KeyType& result = keyMap[t];
            if (result == 0) {
                result = getNextKey();
            }
            return result;
        }

        //special case: treat c-string as std::string
        //needed because char * can't be a key in a map
        KeyType getKey(const char* s) const {
            return getKey(std::string(s));
        }

        template<typename>
        TypeIdType getTypeId() const {
            static auto typeId = getNextTypeId();
            return typeId;
        }

        std::map<TypeIdType, std::map<KeyType, std::shared_ptr<void>>> data;


    public:
        template<typename T, typename CustomKeyType>
        bool has(const CustomKeyType& id) {
            std::lock_guard guard(contextLock);
            const static auto typeId = getTypeId<T>();
            bool ret = data[typeId][getKey(id)] != nullptr;
            if (!ret && parentContext.hasValue()) {
                return parentContext->has<T>(id);
            } else {
                return ret;
            }
        }

        template<typename T>
        bool has() {
            std::lock_guard guard(contextLock);
            const static auto typeId = getTypeId<T>();
            bool ret = data[typeId][getKey(defaultKey)] != nullptr;
            if (!ret && parentContext.hasValue()) {
                return parentContext->has<T>(defaultKey);
            } else {
                return ret;
            }
        }

        /**
         * returns a pointer to a value with this id
         * @tparam T
         * @tparam Vals
         * @param id
         * @param values
         * @return
         */
        template<typename T, typename CustomKeyType>
        Entry<T> get(const CustomKeyType& id) {
            std::lock_guard guard(contextLock);
            const static auto typeId = getTypeId<T>();
            auto ret = std::static_pointer_cast<T>(data[typeId][getKey(id)]);
            if (ret == nullptr) {
                if (parentContext.hasValue()) {
                    return parentContext->get<T, CustomKeyType>(id);
                } else {
                    if constexpr (std::is_convertible_v<CustomKeyType, std::string>) {
                        throw InvalidContextValueException("Invalid context value for key " + id);
                    } else {
                        throw InvalidContextValueException("Invalid context value for key " + std::to_string(id));
                    }
                }
            } else {
                return Entry<T>(ret);
            }


        }

        template<typename T>
        auto get() {
            return get<T>(defaultKey);
        }

        template<typename T>
        auto getSafe() {
            std::lock_guard guard(contextLock);
            if (has<T>()) {
                return get<T>();
            } else {
                return set<T>();
            }

        }

        /**
         * sets context value
         * it disconnects all previous pointers got from get<>
         * @tparam T
         * @tparam Vals
         * @param id
         * @param values
         */

        template<typename T, typename CustomKeyType, typename... Vals>
        void setKey(CustomKeyType id, Vals... values) {
            std::lock_guard<std::recursive_mutex> guard(contextLock);
            static auto typeId = getTypeId<T>();
            data[typeId][getKey(id)] = std::make_shared<T>(values...);
        }

        template<typename ContextValueType, typename... Vals>
        Entry<ContextValueType> set(Vals... values) {
            std::lock_guard<std::recursive_mutex> guard(contextLock);
            //@todo assert that data[typeId][getKey(0)] is null
            static auto typeId = getTypeId<ContextValueType>();
            //std::clog << "Context::setDirect type id " << typeId << std::endl;
            auto ret = std::make_shared<ContextValueType>(values...);
            data[typeId][getKey(defaultKey)] = ret;
            return Entry<ContextValueType>(ret);
        }

        // todo this version may be a problem if someone tries to use the ordinary set and std::shared_ptr is one of vals.
        template<typename ContextValueType, typename RealValueType>
        Entry<ContextValueType> setDirect(std::shared_ptr<RealValueType> valuePtr) {
            std::lock_guard<std::recursive_mutex> guard(contextLock);
            if constexpr (std::is_base_of_v<ContextValueType, RealValueType>) {
                auto ret = std::static_pointer_cast<ContextValueType>(valuePtr);
                static auto typeId = getTypeId<ContextValueType>();
                data[typeId][getKey(defaultKey)] = ret;
                return Entry<ContextValueType>(ret);
            } else {
                using namespace std::string_literals;
                throw InvalidContextException(
                        "Cannot cast "s + typeid(RealValueType).name() + "to " + typeid(ContextValueType).name());
            }
        }

        //    //todo think of a way to combine setDirect and setContext to one function to avoid confusion.
        //    //@todo this should not be named setContext - it can be anything, not just context.
        //    template<typename ContextType, typename ActualContextType, typename... Vals>
        //    auto setContext(Vals... values) {
        //        std::lock_guard<std::recursive_mutex> guard(contextLock);
        //        //@todo assert that data[typeId][getKey(0)] is null
        //        static auto typeId = getTypeId<ContextValueType>();
        //        //std::clog << "Context::setDirect type id " << typeId << std::endl;
        //        auto ret = std::make_shared<ActualContextType>(values...);
        //        data[typeId][getKey(0)] = ret;
        //        return ret;
        //    }

        //@todo non-const context does set parent but const doesn't. WHY?
        //    Context &operator+=(const Context::Ptr other);

        Context& operator+=(const Context::Ptr& other);


        void setDebugId(const std::string& debugId);


    protected:
        Context() = default;

        Context(const Context& other);

        explicit Context(bool defaultContext);

        explicit Context(const Ptr& ptr);

    public:
        static Context::Ptr getActiveContext();

        static bool hasActiveContext();

        static void setActiveContext(Context::Ptr ctx);

        Context::Ptr getParentContext() const;

        void setParentContext(Context::Ptr parentContext);

        static OwnPtr makeContext(bool initialize = true);

        static OwnPtr makeContext(const Context::Ptr& parentContext);

        virtual ~Context();

    private:
        void validateParentContext();
    };

    class SetLocalContext {
        Context::Ptr prevContext;

    public:
        explicit SetLocalContext(Context::Ptr ptr) {
            if (Context::hasActiveContext()) {
                prevContext = Context::getActiveContext();
            }
            Context::setActiveContext(ptr);
        }

        template<typename Func, typename... Args>
        requires std::regular_invocable<Func, Args...> SetLocalContext& operator()(const Func& f, Args... args) {
            f(args...);
            return *this;
        }

        ~SetLocalContext() { Context::setActiveContext(prevContext); }
    };

    class ContextRunner {
    public:
        template<typename Func, typename... Args>
        requires std::regular_invocable<Func, Args...> static void run(const Context::Ptr& context, const Func& f, Args... args) {
            SetLocalContext{context}(f, args...);
        }

        template<typename Func, typename... Args>
        requires std::regular_invocable<Func, Args...> static void run(const Func& f, Args... args) {
            return run(Context::getActiveContext(), f, args...);
        }

        template<typename Func, typename... Args>
        requires std::regular_invocable<Func, Args...> [[nodiscard]] static auto
        runNewThread(const Context::Ptr& context, const Func& f, Args... args) {
            return std::jthread([context, f, args...]() { SetLocalContext{context}(f, args...); });
        }

        template<typename Func, typename... Args>
        requires std::regular_invocable<Func, Args...> [[nodiscard]] static auto runNewThread(const Func& f, Args... args) {
            return runNewThread(Context::getActiveContext(), f, args...);
        }
    };
}// namespace bsc

#endif //BSC_CONTEXT_H
