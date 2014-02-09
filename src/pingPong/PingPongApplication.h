#ifndef PING_PONG_APPLICATION
#define PING_PONG_APPLICATION

#include <Application.h>
#include <IntVTime.h>

class PingPongApplication : public Application {
public:
  PingPongApplication( unsigned int initNumObjects, 
		       unsigned int numEventsPerObject,
		       unsigned int initNumBallsAtOnce,
		       bool initRandomDelays );

  vector<SimulationObject *> *getSimulationObjects(unsigned int numProcessorsAvailable);

  int finalize();

  void registerDeserializers();
  
  const VTime &getPositiveInfinity(){ return IntVTime::getIntVTimePositiveInfinity(); }
  const VTime &getZero(){ return IntVTime::getIntVTimeZero(); }
  const VTime &getTime(string &){ return IntVTime::getIntVTimeZero(); }

private:

  unsigned int numObjects;
  unsigned int numEventsPerObject;
  unsigned int numBallsAtOnce;
  bool randomDelays;
};

#endif
