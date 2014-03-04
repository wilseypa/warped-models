#ifndef RAID_OBJECT_STUB_H
#define RAID_OBJECT_STUB_H

#include "warped.h"
#include "ObjectStub.h"
#include <string>
#include <sstream>

class SimulationObject;

/** The class RAIDProcessStub.
*/
class RAIDProcessStub : public ObjectStub {
public:
   RAIDProcessStub(FactoryImplementationBase *owner) : ObjectStub(owner){}
   ~RAIDProcessStub(){};

   std::string &getName() const {
      static std::string name("RAIDProcess");
      return name;
   }

   const std::string &getInformation() const {
      static std::string info("A Simple RAID Object");
      return info;
   }

   const bool isLocalObject() const {
      return true;
   }

   SimulationObject *createSimulationObject(int numberOfArguments,
                                            std::ostringstream &argumentStream);
};

#endif
