#include "SMMPJoinObject.h"
#include "SMMPJoinState.h"
#include "IntVTime.h"
#include "MemEvents.h"

SMMPJoinObject::SMMPJoinObject(string myName, string initDest, int group):
  myObjectName(myName),
  dest(initDest),
  joinDelay(0) {
    this->group = group;
}

SMMPJoinObject::~SMMPJoinObject(){
   deallocateState(getState());
}

void
SMMPJoinObject::initialize(){
}

void
SMMPJoinObject::finalize(){
}

void
SMMPJoinObject::executeProcess(){
  MemRequest *recvEvent = NULL;
  IntVTime sendTime = static_cast<const IntVTime&> (getSimulationTime());

  while(haveMoreEvents() == true){ 
     recvEvent = (MemRequest*)getEvent();
     if ( recvEvent != NULL ) {
        // send event to server or join or fork or queue as specified by user
        SimulationObject *receiver = getObjectHandle(dest);

        MemRequest *newEvent = new MemRequest(sendTime, sendTime + joinDelay,
                                              this, receiver);
        newEvent->setStartTime(recvEvent->getStartTime());
        newEvent->setProcessor(recvEvent->getProcessor());

        receiver->receiveEvent(newEvent);
     }
  }
}

void
SMMPJoinObject::setJoinDestination(string destination, int delay){
  dest = destination;
  joinDelay = delay ;
}

State*
SMMPJoinObject::allocateState() {
  return new SMMPJoinState();
}

void
SMMPJoinObject::deallocateState(const State *state) {
  delete state;
}

void
SMMPJoinObject::reclaimEvent(const Event *event){
  delete event;
}
