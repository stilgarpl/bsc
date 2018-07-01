//
// Created by stilgar on 01.07.18.
//

#ifndef BASYCO_HIGHLEVELLOGICSOURCE_H
#define BASYCO_HIGHLEVELLOGICSOURCE_H


#include "ISource.h"
#include "LogicManager.h"

class HighLevelLogicSource : public virtual ISource {

    LogicManager *logicManager = nullptr;

    template<typename eventType>
    class WhenHelper {
    public:
        typedef eventType EventType;
        //@todo public or get/set?
    private:
        std::optional<typename EventType::IdType> id;
        HighLevelLogicSource &source;

        friend class HighLevelLogicSource;

    public:
        explicit WhenHelper(HighLevelLogicSource &source) : source(source) {}

        void then() {
            source.then<EventType>();
        }
    };

public:

    template<typename EventType>
    WhenHelper<EventType> when(typename EventType::IdType id) {
        WhenHelper<EventType> result(*this);
        result.id = id;
        return result;
    }

    template<typename EventType>
    void then() {

    }


};


#endif //BASYCO_HIGHLEVELLOGICSOURCE_H
