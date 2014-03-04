#ifndef ADAPTTESTOBJECT_HH
#define ADAPTTESTOBJECT_HH

#include <warped.h>
#include <SimulationObject.h>
#include <vector>
#include <string>
using std::vector;

/**
   This class is the simulation object for the AdaptTest simulation.
   The purpose of this simulation is to induce rollbacks.
*/
class AdaptTestObject : public SimulationObject {
  
public :
  /**
     Constructor
     @param initObjectId      The name of this object
     @param initDestName      Who we're hitting our ball at.
     @param initNumberOfBalls How many balls we expect to receive.
  */
  AdaptTestObject( const int initObjectId,
	           const std::string &destName,
	           const int numberOfBalls,
	           bool adaptiveState,
                   std::string outputMode );
  
  ~AdaptTestObject();
  
  void initialize();
  void finalize();
  
  void executeProcess();
  
  State *allocateState();
  void deallocateState( const State* state );
  
  void reclaimEvent(const Event *event);

  static std::string getName( int forId );

  const std::string &getName() const { return myObjectName; }
  
private :
  const std::string myObjectName;
  const std::string myDestObjectName;
  const int numStragglers;
  SimulationObject *sendTo;
  const bool adaptiveState;
  bool rollbackOccurred;
  int rollbackCount;
  std::string outputMode;
  vector<ObjectID> objects;

  void startEvent();
};

#endif

