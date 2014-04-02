#ifndef D_FLIP_FLOP_STATE_H
#define D_FLIP_FLOP_STATE_H

#include <State.h>

class DFlipFlopState : public State {
public:
    DFlipFlopState(): input(false), previousOutput(false) {}

    void copyState(const State* other) {
        ASSERT(other);
        auto otherState = dynamic_cast<const DFlipFlopState*>(other);
        ASSERT(otherState);
        input = otherState->input;
        previousOutput = otherState->previousOutput;
    }

    bool input;
    bool previousOutput;
};

#endif