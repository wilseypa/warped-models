#ifndef RAID_APPLICATION
#define RAID_APPLICATION

#include "Application.h"
#include <vector>
#include <string>

class RAIDApplication : public Application {
public:
    RAIDApplication(std::string inputFileName, int numObjects);

    std::vector<SimulationObject*>* getSimulationObjects();

    int finalize();

    void registerDeserializers();

private:
    std::string inputFileName;
};

#endif
