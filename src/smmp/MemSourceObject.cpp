#include "MemSourceObject.h"
#include "MemSourceState.h"
#include "IntVTime.h"
using namespace std;

MemSourceObject::MemSourceObject(string initName, int max, int group):
  objectName(initName),
  maxMemRequests(max) {
    this->group = group;
}

MemSourceObject::~MemSourceObject() {
}

void 
MemSourceObject::initialize(){
  //MemSourceState *myState = dynamic_cast<MemSourceState *>( getState() );
  IntVTime sendTime = dynamic_cast<const IntVTime&>(getSimulationTime());

  MemRequest *firstEvent = new MemRequest(sendTime,
                                          sendTime + 1,
                                          this,
                                          this);

  firstEvent->setProcessor(getName());
  firstEvent->setStartTime(0);

  this->receiveEvent(firstEvent);
}

void 
MemSourceObject::finalize(){
   MemSourceState *myState = dynamic_cast<MemSourceState *>(getState());

   cout << getName() << " Complete: " << myState->numMemRequests
        << " requests with recent delay of " << myState->filter.getData() << "\n";
}

void 
MemSourceObject::executeProcess(){
  MemSourceState *myState = static_cast<MemSourceState *>(getState());
  MemRequest* received = NULL;
  IntVTime sendTime = static_cast<const IntVTime&>(getSimulationTime());
  //int id = getObjectID()->getSimulationObjectID();

  while(true == haveMoreEvents()) {
     received = (MemRequest*)getEvent();
     if(received != NULL){
        myState->filter.update((double)((IntVTime &)getSimulationTime() - 
                                        received->getStartTime() ).getTime() );

        int requestsCompleted = myState->numMemRequests;
        if (requestsCompleted < maxMemRequests ) {
           double ldelay = 1.0;

           // we want the memRequest to get there at the exact scheduled time
           SimulationObject *receiver = getObjectHandle(destObjName);
           IntVTime recvTime = sendTime + (int) ldelay;

           MemRequest *newMemRequest = new MemRequest(sendTime,
                                                      recvTime,
                                                      this,
                                                      receiver);

           newMemRequest->setProcessor(getName());
           newMemRequest->setStartTime(sendTime.getTime());

           myState->numMemRequests++;
           myState->oldDelay = sendTime;

           receiver->receiveEvent(newMemRequest);
        }
     }
  }
}

void 
MemSourceObject::setDestination(string dest){
   destObjName = dest;
}

string
MemSourceObject::getDestination(){
   return destObjName;
}

State*
MemSourceObject::allocateState() {
  return new MemSourceState();
}

void
MemSourceObject::deallocateState( const State *state ){
   delete state;
}

void
MemSourceObject::reclaimEvent(const Event *event){
   delete event;
}

const string &
MemSourceObject::getName()const{
  return objectName;
}
