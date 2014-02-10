#ifndef ISCAS85_APPLICATION
#define ISCAS85_APPLICATION

#include "Application.h"
#include "IntVTime.h"
#include <vector>

class Iscas85Application : public Application{
public:

  Iscas85Application(string inputFileName, string testCaseFileName, int numObjects);

  std::vector<SimulationObject*>* getSimulationObjects();

  int finalize();

  void registerDeserializers();

  const VTime &getPositiveInfinity(){return IntVTime::getIntVTimePositiveInfinity();}

  const VTime &getZero(){ return IntVTime::getIntVTimeZero();}

  const VTime &getTime(string &){return IntVTime::getIntVTimeZero();}

private:

  unsigned int numObjects;

  string inputFileName;

  string testCaseFileName;
};

#endif

