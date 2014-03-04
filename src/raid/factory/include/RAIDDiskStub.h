#ifndef RAID_DISK_STUB_H
#define RAID_DISK_STUB_H

#include "warped.h"
#include "ObjectStub.h"
#include <string>
#include <sstream>
class SimulationObject;

/** The class RAIDDiskStub.
*/
class RAIDDiskStub : public ObjectStub {
public:
   RAIDDiskStub(FactoryImplementationBase *owner) : ObjectStub(owner){}
   ~RAIDDiskStub(){};

   std::string &getName() const {
      static std::string name("RAIDDisk");
      return name;
   }

   const std::string &getInformation() const {
      static std::string info("A Simple RAID Disk Object");
      return info;
   }

   const bool isLocalObject() const {
      return true;
   }

   SimulationObject *createSimulationObject(int numberOfArguments,
                                            std::ostringstream &argumentStream);
};

#endif
