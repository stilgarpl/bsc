//
// Created by stilgar on 07.11.17.
//

#ifndef BASYCO_FILESYSTEMMODULE_H
#define BASYCO_FILESYSTEMMODULE_H


#include <p2p/network/node/NodeModule.h>
#include <p2p/dependency/DependencyManaged.h>

class FilesystemModule : public NodeModule, public DependencyManaged<FilesystemModule> {
public:
    FilesystemModule(INode &node);

    void setupActions(LogicManager &logicManager) override;

    bool assignActions(LogicManager &logicManager) override;

    bool setupSources(LogicManager &logicManager) override;

};


#endif //BASYCO_FILESYSTEMMODULE_H
