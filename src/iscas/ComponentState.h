#ifndef COMPONENT_STATE_H
#define COMPONENT_STATE_H

#include <State.h>

class ComponentState : public State {
public:
    ComponentState(unsigned int numberOfInputs) : inputs(numberOfInputs) {}
    
    void copyState(const State* other) {
        ASSERT(other);
        auto otherState = dynamic_cast<const ComponentState*>(other);
        ASSERT(otherState);
        inputs = otherState->inputs;
    }

    std::vector<bool> inputs;
};

#endif