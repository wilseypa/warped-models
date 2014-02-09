#ifndef SMMP_OBJECT_H
#define SMMP_OBJECT_H

#include "SimulationObject.h"

class SMMPObject : public SimulationObject {
public:
    virtual ~SMMPObject() {}

    /// The group number that is used for partitioning
    virtual int getGroup() { return group; };
protected:
    SMMPObject(): group(0) {}

    int group;
};

#endif // SMMP_OBJECT_H
