//
// Created by stilgar on 07.11.17.
//

#include "DependencyManager.h"

DependencyManager::DependencyList DependencyManager::dependencySort(DependencyManager::DependencyList source) {
    DependencyList result;

    while (source.size() > 0) {
        //take first element
        auto &e = source.front();
//            LOGGER("e = " + std::to_string(e->getDepedencyId()));
        //get its dependency list
        ///@todo check if null?
        auto eDeps = e->getRequiredDependencies()->getDependencyIdents();
//            for (auto &&item : eDeps) {
//                std::cout << "edep : " << item << std::endl;
//            }
        //remove fullfilled dependencies from it
        eDeps.erase(
                std::remove_if(eDeps.begin(), eDeps.end(), [&](IDependency::TypeIdType it) -> bool {
                    auto found = std::find_if(result.begin(), result.end(),
                                              [&](std::shared_ptr<IDependencyManaged> jt) -> bool {
                                                  // LOGGER("jt " + std::to_string(jt->getDepedencyId()) + "  it :" + std::to_string(it));
                                                  return jt->getDepedencyId() == it;
                                              });
                    return found != result.end();
                }), eDeps.end());

        //if all dependencies are met, add it to result, remove from source
        if (eDeps.size() == 0) {
            result.push_back(e);
            source.pop_front();
        } else {
            source.push_back(e);
            source.pop_front();
        }
        //if not, element = first unmet dependency
        //loop
    }
    return result;
}
