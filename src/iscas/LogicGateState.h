#ifndef LOGIC_GATE_STATE_H
#define LOGIC_GATE_STATE_H

#include <State.h>

class LogicGateState : public State {
public:
    LogicGateState(unsigned int numberOfInputs) : inputs(numberOfInputs) {}
    
    void copyState(const State* other) {
        ASSERT(other);
        auto otherState = static_cast<const LogicGateState*>(other);
        inputs = otherState->inputs;
    }

    std::vector<bool> inputs;
};

#endif