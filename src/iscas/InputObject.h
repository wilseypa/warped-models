#ifndef INPUT_OBJECT_H
#define INPUT_OBJECT_H

#include <string>

#include "Component.h"

class InputObject : public Component {
public:
    InputObject(std::string name, unsigned int propagationDelay, unsigned int clockPeriod);
    virtual bool needsClock() const { return true; }

    void executeProcess();

    // Input objects obvciously don't support any inputs
    unsigned int addInput();

private:
    std::uniform_int_distribution<int> distribution;
    std::default_random_engine generator;
};

#endif