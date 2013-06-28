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

#ifndef PERSON_H
#define PERSON_H

#define INVALID_PID 0

using namespace std;

class Person {

public:

	Person( unsigned int pid,
			double susceptibility,
			bool isVaccinated,
			string infectionState ) :
		pid(pid),
		susceptibility(susceptibility),
		isVaccinated(isVaccinated),
		infectionState(infectionState) {}

	~Person() {}

	unsigned int pid;

	double susceptibility;

	bool isVaccinated;

	string infectionState;
};

#endif
