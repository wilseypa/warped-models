#ifndef EPIDEMIC_APPLICATION
#define EPIDEMIC_APPLICATION

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the
// U.S., and the terms of this license.

#include <Application.h>
#include <vector>
#include <string>

class EpidemicPartitioner;
class Partitioninfo;
class SimulationObject;

class EpidemicApplication : public Application {

public:

	/* Constructor */
	EpidemicApplication(std::string inputFileName);

    std::vector<SimulationObject*>* getSimulationObjects();
    const PartitionInfo* getPartitionInfo(
        unsigned int numberOfProcessorsAvailable,
        const std::vector<SimulationObject*>* simulationObjects);
	int finalize();

	void registerDeserializers();
  
private:
    std::string inputFileName;
    EpidemicPartitioner* myPartitioner;
};

#endif
