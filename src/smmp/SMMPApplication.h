#ifndef SMMP_APPLICATION
#define SMMP_APPLICATION

#include <Application.h>
#include <IntVTime.h>

#include <vector>

class SimulationObject;

class SMMPApplication : public Application {
public:
  SMMPApplication( string inputFileName );

  std::vector<SimulationObject*>* getSimulationObjects();

  const PartitionInfo* getPartitionInfo(unsigned int numProcessorsAvailable,
                                        const std::vector<SimulationObject*>* simulationObjects);
  
  int finalize();

  void registerDeserializers();
  
  const VTime &getPositiveInfinity(){ return IntVTime::getIntVTimePositiveInfinity(); }
  const VTime &getZero(){ return IntVTime::getIntVTimeZero(); }
  const VTime &getTime(string &){ return IntVTime::getIntVTimeZero(); }

private:
  string inputFileName;
};

#endif
