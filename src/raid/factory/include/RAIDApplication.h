#ifndef RAID_APPLICATION
#define RAID_APPLICATION

#include "Application.h"
#include "IntVTime.h"

class RAIDApplication : public Application {
public:
    RAIDApplication(string inputFileName, int numObjects);

    std::vector<SimulationObject*>* getSimulationObjects();

    int finalize();

    void registerDeserializers();

    const VTime& getPositiveInfinity() { return IntVTime::getIntVTimePositiveInfinity(); }
    const VTime& getZero() { return IntVTime::getIntVTimeZero(); }
    const VTime& getTime(string& time) {
        IntVTime* vtime = new IntVTime(atoi(time.c_str()));
        return *vtime;
    }

private:
    string inputFileName;
};

#endif
