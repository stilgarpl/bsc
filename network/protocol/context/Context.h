//
// Created by stilgar on 20.08.17.
//

#ifndef BASYCO_CONTEXT_H
#define BASYCO_CONTEXT_H


#include <map>
#include <memory>

class Context {

    //@todo template ?
    typedef unsigned int IdType;
    typedef unsigned int TypeIdType;
private:
    TypeIdType getNextId() {
        static IdType val = 0;
        return val++;
    }

    template<typename T>
    TypeIdType getTypeId() {
        static auto typeId = getNextId();
        return typeId;
    };
    std::map<TypeIdType, std::map<IdType, std::shared_ptr<void>>> data;
public:


    /**
     * returns a pointer to a value with this id
     * @tparam T
     * @tparam Vals
     * @param id
     * @param values
     * @return
     */
    template<typename T, typename... Vals>
    std::shared_ptr<T> get(IdType id, Vals... values) {
        static auto typeId = getTypeId<T>();
//        if (data[typeId][id] == nullptr) {
//            data[typeId][id] = std::make_shared<T>(values...);
//        }
        return std::static_pointer_cast<T>(data[typeId][id]);
    }

    template<typename T>
    std::shared_ptr<T> get_(IdType id) {
        static auto typeId = getTypeId<T>();
//        if (data[typeId][id] == nullptr) {
//            data[typeId][id] = std::make_shared<T>(values...);
//        }
        return std::static_pointer_cast<T>(data[typeId][id]);
    }

    /**
     * sets context value
     * it disconnects all previous pointers got from get<>
     * @tparam T
     * @tparam Vals
     * @param id
     * @param values
     */
    template<typename T, typename... Vals>
    void set(IdType id, Vals... values) {
        static auto typeId = getTypeId<T>();
        data[typeId][id] = std::make_shared<T>(values...);
    };


};


#endif //BASYCO_CONTEXT_H
