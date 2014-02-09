#ifndef SMMPJOINOBJECT_HH
#define SMMPJOINOBJECT_HH

#include <iostream>
#include "SidTypes.h"
#include "SMMPObject.h"

class SMMPJoinObject : public SMMPObject {
public:
  
  SMMPJoinObject(string myName, string initDest, int group);
  ~SMMPJoinObject();

  void initialize();
  void finalize();

  void executeProcess();
  void setJoinDestination(string destination, int delay);
  
  State *allocateState();
  void deallocateState( const State* state );
  
  void reclaimEvent(const Event *event);
  const string &getName() const { return myObjectName; }
  
private:
   /// The name of the object.
   string myObjectName;

   /// The name of the destination object.
   string dest;

   /// Used for the delay.
   int joinDelay;
};

#endif 
