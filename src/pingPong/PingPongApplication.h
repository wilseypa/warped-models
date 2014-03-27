#ifndef PING_PONG_APPLICATION
#define PING_PONG_APPLICATION

#include <vector>
#include <Application.h>

class PingPongApplication : public Application {
public:
  PingPongApplication( unsigned int initNumObjects, 
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
