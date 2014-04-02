#ifndef SMMPSERVEROBJECT_HH
#define SMMPSERVEROBJECT_HH

#include <iostream>
#include <string>
#include "rnd/Normal.h"
#include "rnd/Poisson.h"
#include "rnd/Binomial.h"
#include "rnd/Uniform.h"
#include "rnd/NegExp.h"
#include "rnd/Random.h"

#include "SidTypes.h"
#include "SMMPObject.h"

#define SERVERDELAY 20

class SMMPServerObject : public SMMPObject {
public:
  SMMPServerObject(std::string initName, std::string initDest, double initSeed, int group);
  ~SMMPServerObject();
  
  void initialize();
  void finalize();

  void executeProcess();
  void setServerDistribution(distribution_t distribution, double firstParameter,
                             double secondParameter = 0, double (*func)() = NULL);  
  
  // User defined function which will be used for variable delays that
  // Depend upon some state variable.
  double (*userFunction)();
  
  State *allocateState();
  void deallocateState( const State* state );
  
  void reclaimEvent(const Event *event);
  const std::string &getName() const { return myObjectName; }

private:
   /// The name of this object.
   std::string myObjectName;

   /// The name of the destination object.
   std::string dest;

   /// The type of distribution for this object. In this
   /// case, it will always be fixed.
   distribution_t serverDist;

   /// The increment in time from the send time to the receive time.
   double delay;

   /// For this simulation, 'first' just indicates the time between
   /// the send and receive times.
   double first;

   /// Used for other queueing simulations. Not used here.
   double second;

   /// The seed value for random generation.
   double seed;
};

#endif 
