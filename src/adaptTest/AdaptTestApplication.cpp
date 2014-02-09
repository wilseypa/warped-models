#include "AdaptTestApplication.h"
#include "AdaptTestObject.h"
#include "AdaptTestEvent.h"
#include <PartitionInfo.h>
#include <RoundRobinPartitioner.h>
#include <DeserializerManager.h>

#include <tclap/CmdLine.h>
#include <tclap/ValuesConstraint.h>
#include <vector>
using std::string;

AdaptTestApplication::AdaptTestApplication(unsigned int initNumStragglers,
                                           string initOutputMode,
                                           bool initAdaptiveState)
  : numStragglers(initNumStragglers),
    outputMode(initOutputMode),
    adaptiveState(initAdaptiveState) {}

vector<SimulationObject *> *
AdaptTestApplication::getSimulationObjects(unsigned int numProcessorsAvailable){
  vector<SimulationObject *> *retval = new vector<SimulationObject *>;

  for( unsigned int i = 0; i < 2; i++ ){
    retval->push_back( new AdaptTestObject( i,
				            AdaptTestObject::getName( i + 1 ),
				            numStragglers,
				            adaptiveState,
                    outputMode ) );
  }
  retval->push_back( new AdaptTestObject( 2,
                                          AdaptTestObject::getName(2),
                                          numStragglers,
                                          adaptiveState,
                                          outputMode ) );
  
  for( unsigned int j = 3; j < 5; j++ ){

    retval->push_back( new AdaptTestObject( j,
				            AdaptTestObject::getName( j+1 ),
				            numStragglers,
				            adaptiveState,
                                            outputMode ) );
  }
  retval->push_back( new AdaptTestObject( 5,
                                          AdaptTestObject::getName(3),
                                          numStragglers,
                                          adaptiveState,
                                          outputMode ) );
  return retval;
}


int 
AdaptTestApplication::finalize(){ 
  return 0; 
}

void 
AdaptTestApplication::registerDeserializers(){
  DeserializerManager::instance()->registerDeserializer( AdaptTestEvent::getAdaptTestEventDataType(),
							 &AdaptTestEvent::deserialize );

}