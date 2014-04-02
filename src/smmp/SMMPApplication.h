#ifndef SMMP_APPLICATION
#define SMMP_APPLICATION

#include <Application.h>

#include <vector>
#include <string>

class SimulationObject;

class SMMPApplication : public Application {
public:
  SMMPApplication( std::string inputFileName );

  std::vector<SimulationObject*>* getSimulationObjects();

  const PartitionInfo* getPartitionInfo(unsigned int numProcessorsAvailable,
                                        const std::vector<SimulationObject*>* simulationObjects);
  
  int finalize();

  void registerDeserializers();
  
private:
  std::string inputFileName;
};

#endif
