//
// Created by stilgar on 20.08.17.
//

#ifndef BASYCO_CONTEXT_H
#define BASYCO_CONTEXT_H


#include <map>
#include <memory>
#include <iostream>


class Context {


    typedef unsigned int KeyType;
    typedef unsigned int TypeIdType;
private:
    //initialized to nullptr in .cpp file
    thread_local static Context* activeContext;
    //@todo add thread safety - mutex, lock and locking of the parent before accessing it
    Context *parentContext = nullptr;

    const TypeIdType getNextTypeId() const {
        static TypeIdType val = 0;
        return val++;
    }

    const KeyType getNextKey() const {
        static KeyType val = 1;
        return val++;
    }

    ///@todo possible improvement: add key offset for different types, so for example keys "test" and 1 do not point to the same values
    template<typename T>
    const KeyType getKey(const T &t) const {
        static std::map<T, KeyType> keyMap;
        KeyType &result = keyMap[t];
        if (result == 0) {
            result = getNextKey();
        }
        return result;
    }

    //special case: treat c-string as std::string
    //needed because char * can't be a key in a map
    const KeyType getKey(const char *s) const {
        return getKey(std::string(s));
    }

    template<typename>
    const TypeIdType getTypeId() const {
        static auto typeId = getNextTypeId();
        return typeId;
    };
    std::map<TypeIdType, std::map<KeyType, std::shared_ptr<void>>> data;
public:


    /**
     * returns a pointer to a value with this id
     * @tparam T
     * @tparam Vals
     * @param id
     * @param values
     * @return
     */
    template<typename T, typename CustomKeyType>
    std::shared_ptr<T> get(const CustomKeyType &id) {
        static auto typeId = getTypeId<T>();
        auto ret = std::static_pointer_cast<T>(data[typeId][getKey(id)]);
        if (ret == nullptr && parentContext != nullptr) {
            return parentContext->get<T, CustomKeyType>(id);
        } else {
            return ret;
        }
    }

    template<typename T>
    std::shared_ptr<T> get() {
        static auto typeId = getTypeId<T>();
        auto ret = std::static_pointer_cast<T>(data[typeId][getKey(0)]);
        if (ret == nullptr && parentContext != nullptr) {
            return parentContext->get<T>();
        } else {
            return ret;
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
        static auto typeId = getTypeId<T>();
        data[typeId][getKey(id)] = std::make_shared<T>(values...);
    }

    template<typename T, typename... Vals>
    auto set(Vals... values) {
        static auto typeId = getTypeId<T>();
        //std::clog << "Context::set type id " << typeId << std::endl;
        auto ret = std::make_shared<T>(values...);
        data[typeId][getKey(0)] = ret;
        return ret;
    }


    Context &operator+=(const Context &other);

    Context &operator+=(Context &other);

    Context(const Context &other);

    Context() = default;

public:
    static Context &getActiveContext();

    static void setActiveContext(Context *ctx);

    Context *getParentContext() const;

    void setParentContext(Context *parentContext);
};


#endif //BASYCO_CONTEXT_H
