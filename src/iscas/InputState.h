#ifndef INPUT_STATE_H
#define INPUT_STATE_H

#include <State.h>
#include <random>
#include <functional>

class InputState : public State {
public:
    InputState(std::string objectName)
        : generator(std::hash<std::string>()(objectName)) {}

    void copyState(const State* other) {
        ASSERT(other);
        auto otherState = static_cast<const InputState*>(other);
        generator = otherState->generator;
    }

    std::default_random_engine generator;
};

#endif