#ifndef ISCAS85_APPLICATION
#define ISCAS85_APPLICATION

#include "warped/Application.h"
#include "warped/IntVTime.h"

class Iscas85Application : public Application{
public:

  Iscas85Application(string inputFileName, string testCaseFileName, int numObjects);

  int getNumberOfSimulationObjects(int mgrId) const;

  const PartitionInfo *getPartitionInfo( unsigned int numberOfProcessorsAvailable);

  int finalize();

  void registerDeserializers();

  string getCommandLineParameters() const {return "foo"; }

  const VTime &getPositiveInfinity(){return IntVTime::getIntVTimePositiveInfinity();}

  const VTime &getZero(){ return IntVTime::getIntVTimeZero();}

  const VTime &getTime(string &){return IntVTime::getIntVTimeZero();}

private:

  unsigned int numObjects;

  string inputFileName;

  string testCaseFileName;
};

#endif

