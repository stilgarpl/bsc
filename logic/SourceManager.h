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
private:
    std::list<SourcePtr> sources;

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

};


#endif //BASYCO_SOURCEMANAGER_H
