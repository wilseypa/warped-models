#ifndef MEMROUTEROBJECT_HH
#define MEMROUTEROBJECT_HH

#include "SMMPObject.h"
#include <string>
/* The MemRouterObject Class.
   
   This class takes incoming memory requests and routes them back to
   main source processor object that sent the request.

*/
class MemRouterObject : public SMMPObject  {
public:
  MemRouterObject(std::string initName, int group);
  ~MemRouterObject();

  void initialize();
  void finalize();
  
  void executeProcess();
  
  State *allocateState();
  void deallocateState( const State* state );
  
  void reclaimEvent(const Event *event);

  const std::string &getName() const;

private:
   std::string objectName;
};

#endif
