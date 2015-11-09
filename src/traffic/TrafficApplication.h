#ifndef TRAFFIC_APPLICATION_H
#define TRAFFIC_APPLICATION_H

#include <vector>
#include <Application.h>

class TrafficApplication : public Application {
public:
  TrafficApplication( unsigned int initNumObjects, 
		       unsigned int numEventsPerObject,
		       unsigned int initNumBallsAtOnce,
		       bool initRandomDelays );

  std::vector<SimulationObject *> *getSimulationObjects();

  void registerDeserializers();
  
private:

  unsigned int numObjects;
  unsigned int numEventsPerObject;
  unsigned int numBallsAtOnce;
  bool randomDelays;
};

#endif
