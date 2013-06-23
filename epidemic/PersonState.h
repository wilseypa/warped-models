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

#ifndef PERSONSTATE_H
#define PERSONSTATE_H

#include "State.h"
#include "IntVTime.h"

using namespace std;

class PersonState : public State {

public:

	/* Default constructor */
	PersonState();

	/* Destructor */
	~PersonState();

	/* Copy the state */
	void copyState(const State* state);

	/* Get the state size */
	unsigned int getStateSize() const;

private:

	/* Person type and/or traits */
	float        suceptibility;

	/* Infection details */
	unsigned int currInfectState;
	unsigned int prevInfectState;
	unsigned int infectionDuration;

	/* Global identifier */
	unsigned int pid;
};

#endif
