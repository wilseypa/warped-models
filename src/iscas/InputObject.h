#ifndef INPUT_OBJECT_H
#define INPUT_OBJECT_H

#include <string>
#include <random>

#include "Component.h"

class InputObject : public Component {
public:
    InputObject(std::string name, unsigned int propagationDelay, unsigned int clockPeriod);
    virtual bool needsClock() const { return true; }

    void executeProcess();
    State* allocateState();

private:
    std::uniform_int_distribution<int> distribution;
};

#endif