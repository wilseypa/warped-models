#ifndef RAID_FORK_STUB_H
#define RAID_FORK_STUB_H

#include "warped.h"
#include "ObjectStub.h"
#include <string>
#include <sstream>
class SimulationObject;

/** The class RAIDForkStub.
*/
class RAIDForkStub : public ObjectStub {
public:
   RAIDForkStub(FactoryImplementationBase *owner) : ObjectStub(owner){}
   ~RAIDForkStub(){};

   std::string &getName() const {
      static std::string name("RAIDFork");
      return name;
   }

   const std::string &getInformation() const {
      static std::string info("A Simple RAID Fork Object");
      return info;
   }

   const bool isLocalObject() const {
      return true;
   }

   SimulationObject *createSimulationObject(int numberOfArguments,
                                            std::ostringstream &argumentStream);
};

#endif
