#ifndef PHOLDPROCESSSTATE_H
#define PHOLDPROCESSSTATE_H

// See copyright notice in file Copyright in the root directory of this archive.

#include "warped.h"
#include "../rnd/MLCG.h"
#include "State.h"

class ProcessState : public State {
   friend ostream& operator<< (ostream&, const ProcessState&);
public:
   MLCG *gen;
   int sizeOfStateData;
   char *stateBulk;
   
   ProcessState();
   ProcessState(int size);
   ProcessState( const ProcessState& );
   ~ProcessState();

   void eventSent(){ numSent++; }
   void eventReceived(){ numReceived++; }

   int getNumSent(){ return numSent; }
   int getNumReceived(){ return numReceived; }
   
   // this is needed to copy User State
   void copyState(const State* state);
   
   unsigned int getStateSize() const { return sizeof(ProcessState); }

private:
   int numSent;
   int numReceived;
};

#endif

