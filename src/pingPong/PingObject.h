#ifndef PINGOBJECT_HH
#define PINGOBJECT_HH

#include <warped.h>
#include <SimulationObject.h>

/**
   This class is the simulation object for the Ping Pong simulation.  The
   entire point of this simulation is to have pathologically low
   granularity.  Don't expect to get parallel speedup on this one.  :-)
*/
class PingObject : public SimulationObject {
  
public :
  /**
     Constructor
     @param initObjectId      The name of this object
     @param initDestName      Who we're hitting our ball at.
     @param initNumberOfBalls How many balls we expect to receive.
  */
  PingObject( const int initObjectId,
	      const std::string &destName,
	      const int numberOfBalls,
	      bool master,
	      bool randomDelays );
  
  ~PingObject();
  
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
  const int numBalls;
  SimulationObject *sendTo;
  const bool isMaster;
  const bool randomDelays;
  double lastX;

  /**
     Handles the sending of our event to our neighbor.
  */
  void sendEvent( const std::string &ownerName );

  /**
     Starts a new "ball" in the system.
  */
  void startBall();
};

#endif
