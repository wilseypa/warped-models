#ifndef ADAPT_TEST_APPLICATION
#define ADAPT_TEST_APPLICATION

#include <Application.h>
#include <IntVTime.h>

class AdaptTestApplication : public Application {
public:
  AdaptTestApplication( unsigned int numStragglers,
          		          string initOutputMode,
          		          bool initAdaptiveState );

  std::vector<SimulationObject*>* getSimulationObjects();
  
  int finalize();

  void registerDeserializers();
  
  const VTime &getPositiveInfinity(){ return IntVTime::getIntVTimePositiveInfinity(); }
  const VTime &getZero(){ return IntVTime::getIntVTimeZero(); }
  const VTime &getTime(string &){ return IntVTime::getIntVTimeZero(); }

private:
  unsigned int numObjects;
  unsigned int numStragglers;
  string outputMode;
  bool adaptiveState;
};

#endif

