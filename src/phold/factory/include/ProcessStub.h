#ifndef PHOLD_OBJECT_STUB_H
#define PHOLD_OBJECT_STUB_H

#include "warped.h"
#include "ObjectStub.h"
#include <vector>
#include <sstream>

class SimulationObject;

class ProcessStub : public ObjectStub {
public:
   ProcessStub(FactoryImplementationBase *owner) : ObjectStub(owner){}
   ~ProcessStub(){};

   std::string &getName() const {
      static std::string name("Process");
      return name;
   }

   const std::string &getInformation() const {
      static std::string info("A Simple PHOLD Object");
      return info;
   }

   const bool isLocalObject() const {
      return true;
   }

   SimulationObject *createSimulationObject(int numberOfArguments,
                                            std::ostringstream &argumentStream);
};

#endif
