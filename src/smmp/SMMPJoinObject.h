#ifndef SMMPJOINOBJECT_HH
#define SMMPJOINOBJECT_HH

#include <iostream>
#include <string>
#include "SidTypes.h"
#include "SMMPObject.h"

class SMMPJoinObject : public SMMPObject {
public:
  
  SMMPJoinObject(std::string myName, std::string initDest, int group);
  ~SMMPJoinObject();

  void initialize();
  void finalize();

  void executeProcess();
  void setJoinDestination(std::string destination, int delay);
  
  State *allocateState();
  void deallocateState( const State* state );
  
  void reclaimEvent(const Event *event);
  const std::string &getName() const { return myObjectName; }
  
private:
   /// The name of the object.
   std::string myObjectName;

   /// The name of the destination object.
   std::string dest;

   /// Used for the delay.
   int joinDelay;
};

#endif 
