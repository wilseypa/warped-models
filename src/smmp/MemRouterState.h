#ifndef MEMROUTERSTATE_HH
#define MEMROUTERSTATE_HH

#include "State.h"

class MemRouterState : public State {
public:
 
   MemRouterState(){}; 
   ~MemRouterState(){};

   void copyState(const State *toCopy){};
};

#endif
