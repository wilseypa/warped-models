// Copyright (c) Clifton Labs, Inc.
// All rights reserved.

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, RESULT
// OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the
// U.S., and the terms of this license.

// Authors: Malolan Chetlur             mal@ececs.uc.edu
//          Jorgen Dahl                 dahlj@ececs.uc.edu
//          Dale E. Martin              dmartin@ececs.uc.edu
//          Radharamanan Radhakrishnan  ramanan@ececs.uc.edu
//          Dhananjai Madhava Rao       dmadhava@ececs.uc.edu
//          Philip A. Wilsey            phil.wilsey@uc.edu
//          Randall King                kingr8@mail.uc.edu

#include "../include/RAIDApplication.h"
#include "../include/RAIDProcess.h"
#include "../include/RAIDFork.h"
#include "../include/RAIDDisk.h"
#include "../include/RAIDRequest.h"
#include <warped/PartitionInfo.h>
#include <warped/RoundRobinPartitioner.h>
#include <warped/DeserializerManager.h>
#include <utils/ArgumentParser.h>

#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
using std::string;

RAIDApplication::RAIDApplication()
  : numObjects( 0 ) {}

int 
RAIDApplication::initialize( vector<string> &arguments ){ 
  getArgumentParser().checkArgs( arguments );

  if( inputFileName.empty() ){
    std::cerr << "A raidSim configuration file must be specified using -simulate" << std::endl;
    abort();
  }

  return 0;
}

int 
RAIDApplication::getNumberOfSimulationObjects(int mgrId) const { 
  return numObjects;
}

vector<SimulationObject *> *
RAIDApplication::getSimulationObjects(){
  vector<SimulationObject *> *retval = new vector<SimulationObject *>;

  string name;        // Used for the fork, disk, and process names.
  string stType;      // Stores the disk type as a string.
  DISK_TYPE type;     // Used for the disk type.
  int numOutputs;     // Used for fork number of outputs.
  int totalNumDisks;  // The total number of disks in the simulation.
  int numForks;       // Number of forks in simulation.
  int numProcesses;   // Number of processes in simulation.
  int startDisk;      // Number of the first disk for the fork.
  string outputFork;  // Name of the output fork for a process.
  int maxNumRequests; // Number of requests to be made by process.
  string temp;

  ifstream configFile;
  configFile.open( inputFileName.c_str() );

  if(configFile.fail()){
    cerr << "Could not open file: '" << inputFileName << "'" << endl;
    cerr << "Terminating simulation." << endl;
    abort();
  }
  
  // Read in the number of disks, forks, and processes.
  configFile >> totalNumDisks >> numForks >> numProcesses;
  numObjects = totalNumDisks + numForks + numProcesses;

  // Setup the disks.
  for( int d = 0; d < totalNumDisks; d++){
    configFile >> name >> stType;
    
    if(stType == "FUJITSU"){
      type = FUJITSU;
    }
    else if(stType == "FUTUREDISK"){
      type = FUTUREDISK;
    }
    else if(stType == "LIGHTNING"){
      type = LIGHTNING;
    }
    else{
      cerr << "Invalid disktype: Must be FUJITSU, FUTUREDISK, or LIGHTNING." << endl;
      cerr << "Terminating simulation." << endl;
      abort();
    }
    
    retval->push_back( new RAIDDisk( name, type ) );
  }

  // Setup the forks,
  for( int f = 0; f < numForks; f++){
    configFile >> name >> numOutputs;
    vector<string> outputNames;  // Used for storing fork output disk names.
    
    for( int n = 0; n < numOutputs; n++){
      configFile >> temp;
      outputNames.push_back(temp);
    }

    configFile >> startDisk;

    retval->push_back( new RAIDFork( name, numOutputs, outputNames, totalNumDisks, startDisk ) );
  }
    
  for( int p = 0; p < numProcesses; p++){
    configFile >> name >> outputFork >> stType >> maxNumRequests >> startDisk;

    if(stType == "FUJITSU"){
      type = FUJITSU;
    }
    else if(stType == "FUTUREDISK"){
      type = FUTUREDISK;
    }
    else if(stType == "LIGHTNING"){
      type = LIGHTNING;
    }
    else{
      cerr << "Invalid disktype: Must be FUJITSU, FUTUREDISK, or LIGHTNING." << endl;
      cerr << "Terminating simulation." << endl;
      abort();
    }
 
    retval->push_back( new RAIDProcess( name, outputFork, totalNumDisks, type, maxNumRequests, startDisk, p ) );
  }

  configFile.close();
  
  return retval;
}

const PartitionInfo *
RAIDApplication::getPartitionInfo( unsigned int numberOfProcessorsAvailable ){
  const PartitionInfo *retval = 0;

  Partitioner *myPartitioner = new RoundRobinPartitioner();
  // Now we'll create some simulation objects...
  vector<SimulationObject *> *objects = getSimulationObjects();
  retval = myPartitioner->partition( objects, numberOfProcessorsAvailable );
  delete objects;

  return retval;
}

int 
RAIDApplication::finalize(){ 
  return 0; 
}

void 
RAIDApplication::registerDeserializers(){
  DeserializerManager::instance()->registerDeserializer( RAIDRequest::getRAIDRequestDataType(),
							 &RAIDRequest::deserialize );
}

ArgumentParser &
RAIDApplication::getArgumentParser(){
  static ArgumentParser::ArgRecord args[] = {
    { "-simulate", "raidSim configuration file name", &inputFileName, ArgumentParser::STRING, false },

    { "", "", 0 }
  };
  
  static ArgumentParser *myArgParser = new ArgumentParser( args );
  return *myArgParser;
}
