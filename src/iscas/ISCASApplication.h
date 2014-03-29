#ifndef ISCAS_APPLICATION_H
#define ISCAS_APPLICATION_H

#include <Application.h>
#include <IntVTime.h>
#include <string>
#include <vector>

class SimulationObject;

class ISCASApplication : public Application {
public:
    ISCASApplication(std::string benchFileName, unsigned int propagationDelay,
                     unsigned int clockPeriod);

    std::vector<SimulationObject*>* getSimulationObjects();
    void registerDeserializers();

private:
    std::string benchFileName;
    unsigned int propagationDelay;
    unsigned int clockPeriod;
};


#endif