#ifndef PINGOBJECTSTATE_H
#define PINGOBJECTSTATE_H

#include <State.h>
#include <sstream>
#include <string>

class PingObjectState : public State {
public :
  PingObjectState() :
    numBallsStarted( 0 ),
    numBallsRecvd( 0 ),
    numBallsSent( 0 ){};

  ~PingObjectState(){};
  
  void copyState( const State *toCopy ){
    ASSERT( toCopy != 0 );
    const PingObjectState *pingState = dynamic_cast<const PingObjectState *>(toCopy);
    numBallsRecvd = pingState->numBallsRecvd;
    numBallsSent = pingState->numBallsSent;
    numBallsStarted = pingState->numBallsStarted;
  }

  void ballStarted(){ numBallsStarted++; }
  void ballSent(){ numBallsSent++; }
  void ballReceived(){ numBallsRecvd++; }

  int getNumStarted(){ return numBallsStarted; }

  const std::string getSummaryString(){
   std::ostringstream oss;
   oss << "received " << numBallsRecvd << ", sent " << numBallsSent << ", and started " << numBallsStarted;
   return oss.str();
  }

private:
  int numBallsStarted;
  int numBallsRecvd;
  int numBallsSent;
};

#endif
