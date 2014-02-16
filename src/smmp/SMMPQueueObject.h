#ifndef SMMPQUEUEOBJECT_HH
#define SMMPQUEUEOBJECT_HH

#include <iostream>
#include "SMMPObject.h"

class SMMPQueueObject : public SMMPObject {
public: 
   SMMPQueueObject(string initName, string initServerName, int group);
  ~SMMPQueueObject();

   void initialize();
   void finalize();
 
   void executeProcess();

   State *allocateState();
   void deallocateState( const State* state );
  
   void reclaimEvent(const Event *event);
   const string &getName() const { return myObjectName; } 

private:
   /// The name of this object.
   string myObjectName;

   /// The name of the destination object.
   string serverName;
};

#endif 
