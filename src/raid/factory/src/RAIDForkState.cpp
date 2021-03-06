//-*-c++-*-
#include "../include/RAIDForkState.h"
using namespace std;

RAIDForkState::RAIDForkState()  {
  lastOutput = 0;
} 

void
RAIDForkState::copyState(const State* state) {
  this->lastOutput = ((RAIDForkState *) state)->lastOutput;
}

const SerializedInstance*
RAIDForkState::serialize() {
  cerr << "Error: RAIDForkState::serialize called\n";
  return NULL;
} // End of serialize()

State*
RAIDForkState::deserialize(const SerializedInstance instance) {
  cerr << "Error: RAIDForkState::deserialize called\n";
  return NULL;
} // End of deserialize()

std::ostream& RAIDForkState::printState(std::ostream& os) {
  os << "Fork::lastOutput: " << this->lastOutput; 
  return os;
}
