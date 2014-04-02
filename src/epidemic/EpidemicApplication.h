#ifndef EPIDEMIC_APPLICATION
#define EPIDEMIC_APPLICATION

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the
// U.S., and the terms of this license.

#include <Application.h>
#include <IntVTime.h>
#include <vector>

class EpidemicPartitioner;
class Partitioninfo;
class SimulationObject;

class EpidemicApplication : public Application {

public:

	/* Constructor */
	EpidemicApplication(string inputFileName);

    std::vector<SimulationObject*>* getSimulationObjects();
    const PartitionInfo* getPartitionInfo(
        unsigned int numberOfProcessorsAvailable,
        const std::vector<SimulationObject*>* simulationObjects);
	int finalize();

	void registerDeserializers();
  
	const VTime &getPositiveInfinity(){ return IntVTime::getIntVTimePositiveInfinity(); }

	const VTime &getZero(){ return IntVTime::getIntVTimeZero(); }

	const VTime &getTime(string &time ){ 
		IntVTime *vtime = new IntVTime (atoi(time.c_str())); 
		return *vtime;
	}

private:
    string inputFileName;
    EpidemicPartitioner* myPartitioner;
};

#endif
