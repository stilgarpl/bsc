//
// Created by stilgar on 20.08.17.
//

#ifndef BASYCO_CONTEXT_H
#define BASYCO_CONTEXT_H


#include <map>
#include <memory>

class Context {

private:
    unsigned int getNextId() {
        static unsigned int val = 0;
        return val++;
    }

    template<typename T>
    unsigned int getTypeId() {
        static auto typeId = getNextId();
        return typeId;
    };
    std::map<unsigned int, std::map<unsigned int, std::shared_ptr<void>>> data;
public:


    template<typename T, typename... Vals>
    std::shared_ptr<T> get(unsigned int id, Vals... values) {
        static auto typeId = getTypeId<T>();
        if (data[typeId][id] == nullptr) {
            data[typeId][id] = std::make_shared<T>(values...);
        }
        return std::static_pointer_cast<T>(data[typeId][id]);
    }

};


#endif //BASYCO_CONTEXT_H
