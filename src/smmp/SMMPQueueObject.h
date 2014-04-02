#ifndef SMMPQUEUEOBJECT_HH
#define SMMPQUEUEOBJECT_HH

#include <iostream>
#include <string>
#include "SMMPObject.h"

class SMMPQueueObject : public SMMPObject {
public: 
   SMMPQueueObject(std::string initName, std::string initServerName, int group);
  ~SMMPQueueObject();

   void initialize();
   void finalize();
 
   void executeProcess();

   State *allocateState();
   void deallocateState( const State* state );
  
   void reclaimEvent(const Event *event);
   const std::string &getName() const { return myObjectName; } 

private:
   /// The name of this object.
   std::string myObjectName;

   /// The name of the destination object.
   std::string serverName;
};

#endif 
