// Copyright (c) The University of Cincinnati.  
// All rights reserved.

// UC MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF 
// THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  UC SHALL NOT BE LIABLE
// FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING,
// RESULT OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS
// DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the
// U.S., and the terms of this license.

// Authors: Malolan Chetlur             mal@ececs.uc.edu
//          Jorgen Dahl                 dahlj@ececs.uc.edu
//          Dale E. Martin              dmartin@ececs.uc.edu
//          Radharamanan Radhakrishnan  ramanan@ececs.uc.edu
//          Dhananjai Madhava Rao       dmadhava@ececs.uc.edu
//          Philip A. Wilsey            phil.wilsey@uc.edu

//---------------------------------------------------------------------------
// 
// $Id: RAIDForkStub.cpp
// 
//---------------------------------------------------------------------------

#include "../include/RAIDFork.h"
#include "../include/RAIDForkStub.h"
#include "SimulationObject.h"

SimulationObject*
RAIDForkStub::createSimulationObject(int numberOfArguments,
                                       ostringstream &argumentStream) {
  RAIDFork *newObject;
  
  /* NR OF REQUIRED OBJECTS NOT YET DETERMINED!
     if (numberOfArguments < 6) {
       cerr << "Invalid number of parameters for Process" << endl;
       cerr << "Minimum is 6 but got " << numberOfArguments << endl;
       cerr << "Argument Stream = " << argumentStream.str() << endl;
       delete [] argumentStream.str();
       exit(-1);
     }
  */

  istringstream inputStream(argumentStream.str());
  
  string name;
  int nrOutputs;
  int dummyOutPortNr;
  vector<string> outputNames;
  int dummyInPortNrOfDest;
  int disks;
  int startDisk;

  // Break out the input stream into separate fields
  inputStream >> name >> nrOutputs;

  // Pack the outputNames vector with the output names. 
  int iter = 0;
  string tmp;
  while (iter < nrOutputs) {
    inputStream >> dummyOutPortNr >> tmp >> dummyInPortNrOfDest;
    outputNames.push_back(tmp);
    iter++;
  }
  inputStream >> disks >> startDisk;

  newObject = new RAIDFork(name, nrOutputs, outputNames, disks, startDisk);

  //delete [] argumentStream.str();

  return (SimulationObject *) newObject;
}
