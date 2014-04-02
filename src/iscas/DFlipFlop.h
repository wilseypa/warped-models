#ifndef D_FLIP_FLOP_H
#define D_FLIP_FLOP_H

#include <string>

#include "Component.h"

class DFlipFlop : public Component {
public:
    DFlipFlop(std::string name, unsigned int propagationDelay, unsigned int clockPeriod);

    virtual bool needsClock() const { return true; }

    void executeProcess();
    State* allocateState();
};

#endif