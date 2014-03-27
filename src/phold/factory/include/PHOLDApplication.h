#ifndef PHOLD_APPLICATION
#define PHOLD_APPLICATION

#include <Application.h>
#include <vector>
#include <string>

class PHOLDApplication : public Application {
public:
  PHOLDApplication(std::string inputFileName, int numObjects);

  std::vector<SimulationObject*>* getSimulationObjects();
  
  int finalize();

  void registerDeserializers();
  
private:
  std::string inputFileName;
  int numObjects;
  int numLPs;
};

#endif
