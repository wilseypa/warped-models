#include "PingPongApplication.h"
#include "PingObject.h"
#include "PingEvent.h"

#include <DeserializerManager.h>

#include <vector>
using std::string;
using std::vector;

PingPongApplication::PingPongApplication( unsigned int initNumObjects,
					  unsigned int initNumEventsPerObject,
					  unsigned int initNumBallsAtOnce,
					  bool initRandomDelays ) 
  : numObjects( initNumObjects ),
    numEventsPerObject( initNumEventsPerObject ),
    numBallsAtOnce( initNumBallsAtOnce ),
    randomDelays( initRandomDelays ){}

vector<SimulationObject *> *
PingPongApplication::getSimulationObjects(){
  vector<SimulationObject *> *retval = new vector<SimulationObject *>;
  retval->reserve( numObjects );

  // The first and last objects are special...
  int mastersLeft = numBallsAtOnce;
  bool master = false;
  for( unsigned int i = 0; i < numObjects - 1; i++ ){
    if( mastersLeft-- > 0 ){
      master = true;
    }
    else{
      master = false;
    }
    retval->push_back( new PingObject( i,
				       PingObject::getName( i + 1 ),
				       numEventsPerObject,
				       master,
				       randomDelays
				       ) );
  }
  if( mastersLeft-- > 0 ){
    master = true;
  }
  else{
    master = false;
  }
  retval->push_back( new PingObject( numObjects - 1,
				     PingObject::getName( 0 ),
				     numEventsPerObject,
				     master,
				     randomDelays ) );
  
  return retval;
}


int 
PingPongApplication::finalize(){ 
  return 0; 
}

void 
PingPongApplication::registerDeserializers(){
  DeserializerManager::instance()->registerDeserializer( PingEvent::getPingEventDataType(),
							 &PingEvent::deserialize );

}

