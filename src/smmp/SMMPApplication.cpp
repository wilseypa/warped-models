#include "SMMPApplication.h"
#include "MemEvents.h"
#include "MemSourceObject.h"
#include "MemRouterObject.h"
#include "SMMPForkObject.h"
#include "SMMPJoinObject.h"
#include "SMMPServerObject.h"
#include "SMMPQueueObject.h"
#include "SMMPPartitioner.h"
#include "PartitionInfo.h"
#include "DeserializerManager.h"
#include "SimulationObject.h"
#include "SMMPObject.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

#define NUMPER 6

SMMPApplication::SMMPApplication(string inputFileName)
    : inputFileName(inputFileName) {}

const PartitionInfo* 
SMMPApplication::getPartitionInfo(unsigned int numProcessorsAvailable,
                                  const std::vector<SimulationObject*>* simulationObjects){
  SMMPPartitioner *myPartitioner = new SMMPPartitioner();

  map<int, vector<SimulationObject*>> groups;

  for (auto obj : *simulationObjects) {
    groups[static_cast<SMMPObject*>(obj)->getGroup()].push_back(obj);
  }

  for (auto& mapiter : groups) {
    myPartitioner->addObjectGroup(&(mapiter.second));
  }

  // Perform the actual partitioning of groups.
  return myPartitioner->partition( NULL, numProcessorsAvailable );
}

std::vector<SimulationObject*>*
SMMPApplication::getSimulationObjects(unsigned int numProcessorsAvailable){
  int numProcs;
  int cacheSpeed;
  double cacheHitRatio;
  int mainSpeed;
  int numReqPerProc;

  ifstream configFile;
  configFile.open( inputFileName.c_str() );

  if(configFile.fail()){
    cerr << "Could not open file: '" << inputFileName << "'" << endl;
    cerr << "Terminating simulation." << endl;
    abort();
  }

  configFile >> numProcs >> cacheSpeed >> cacheHitRatio >> mainSpeed >> numReqPerProc;
  configFile.close();

  // These names will be used for each object type. A number will be appended to create
  // unique names.
  string sourceName = "Source.";
  string join1Name = "Join1.";
  string queueName = "Queue.";
  string forkName = "Fork.";
  string serverName = "Server.";
  string join2Name = "Join2.";

  // This is the name of the final join object that is used for the main memory.
  stringstream strstm;
  strstm << numProcs * NUMPER;
  string finalJoin = "Join.F";

  // Used to initialize the seeds for the objects.
  double seed = 0;

  vector<SimulationObject*> * simulationObjects = new vector<SimulationObject*>;

  // For each processor in the simulation, initialize 6 objects.
  int p = 0;
  for(; p < numProcs; p++){
    stringstream out;
    out << p;
    string curProc = out.str();

    MemSourceObject *source = new MemSourceObject(sourceName + curProc, numReqPerProc, p);
    source->setDestination(join1Name + curProc);
    simulationObjects->push_back(source);

    SMMPJoinObject *join1 = new SMMPJoinObject(join1Name + curProc, queueName + curProc, p);
    simulationObjects->push_back(join1);

    SMMPQueueObject *queue = new SMMPQueueObject(queueName + curProc, forkName + curProc, p);
    simulationObjects->push_back(queue);

    vector<string> outputs(2,"");
    outputs[0] = serverName + curProc;    
    outputs[1] = finalJoin;
    SMMPForkObject *fork = new SMMPForkObject(forkName + curProc, seed++, cacheHitRatio, outputs, p);
    simulationObjects->push_back(fork);

    SMMPServerObject *server = new SMMPServerObject(serverName + curProc, join2Name + curProc, seed++, p);
    server->setServerDistribution(FIXED, cacheSpeed);
    simulationObjects->push_back(server);

    SMMPJoinObject *join2 = new SMMPJoinObject(join2Name + curProc, sourceName + curProc, p);
    simulationObjects->push_back(join2);
  }
  p += 1;

  // Then initialize the final 4 objects for the main memory.
  string finalQueue = "Queue.F";
  string finalServer = "Server.F";
  string finalMemRouter = "MemRouter.F";

  SMMPJoinObject *joinF = new SMMPJoinObject(finalJoin, finalQueue, p);
  simulationObjects->push_back(joinF);

  SMMPQueueObject *queueF = new SMMPQueueObject(finalQueue, finalServer, p);
  simulationObjects->push_back(queueF);

  SMMPServerObject *serverF = new SMMPServerObject(finalServer, finalMemRouter, seed++, p);
  serverF->setServerDistribution(FIXED, mainSpeed);
  simulationObjects->push_back(serverF);

  MemRouterObject *memRouterF = new MemRouterObject(finalMemRouter, p);
  simulationObjects->push_back(memRouterF);

  return simulationObjects;
}

int
SMMPApplication::finalize(){
  return 0;
}

void
SMMPApplication::registerDeserializers(){
  DeserializerManager::instance()->registerDeserializer( MemRequest::getMemRequestDataType(),
							 &MemRequest::deserialize );
}