#include "DFFInitializer.h"
#include "InitializerState.h"

using namespace std;

DFFInitializer::DFFInitializer(string &objectName, const int numOutPuts,vector<string>* outputs,
                               vector<int>* desInputPorts):thisObjectName(objectName),
                               numberOfOutputs(numOutPuts),outPuts(outputs),Ports(desInputPorts),
                               outputHandles(NULL){}

DFFInitializer::~DFFInitializer(){
  if(numberOfOutputs != 0){
    delete outPuts;
    delete [] outputHandles;
    delete Ports;
  }
}

void
DFFInitializer::initialize(){
  InitializerState *newState =(InitializerState *) getState();
  if (newState !=NULL)
  if(numberOfOutputs != 0){
    outputHandles = new SimulationObject *[numberOfOutputs];
    for(int i = 0; i < numberOfOutputs; i++){
      outputHandles[i] = getObjectHandle((*outPuts)[i]);
    }

    IntVTime sendTime = static_cast<const IntVTime&>(getSimulationTime());
    IntVTime recvTime = sendTime + 1.0;

    for(int j = 0; j <2; j++){
      LogicEvent *event = new LogicEvent(sendTime,recvTime,this,outputHandles[j]);
      event->setbitValue(j);
      event->setsourcePort(0);
      event->setdestinationPort(2); 
      outputHandles[j]->receiveEvent(event);
    }

  }
}

void 
DFFInitializer::finalize(){}

void 
DFFInitializer::executeProcess(){}

State*
DFFInitializer::allocateState(){
  return (State*) new InitializerState();
}

void
DFFInitializer::deallocateState(const State* state){
  delete (InitializerState *) state;
}

void 
DFFInitializer::reclaimEvent(const Event *event){
  delete (LogicEvent* )event;
}

void
DFFInitializer::reportError(const string& msg, SEVERITY level){}


