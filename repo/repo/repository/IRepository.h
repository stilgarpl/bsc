//
// Created by stilgar on 22.12.17.
//

#ifndef BASYCO_IREPOSITORY_H
#define BASYCO_IREPOSITORY_H


#include <memory>


class IRepository {
public:
    typedef std::string RepoIdType;

    virtual const RepoIdType &getRepositoryId() const =0;
};


#endif //BASYCO_IREPOSITORY_H
