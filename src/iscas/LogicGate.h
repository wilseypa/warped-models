#ifndef LOGIC_GATE_H
#define LOGIC_GATE_H

#include <string>

#include "Component.h"

class State;

class LogicGate : public Component {
public:
    enum class Type {NOT, AND, OR, XOR, NAND, NOR};

    LogicGate(std::string name, unsigned int propagationDelay,
              unsigned int clockPeriod, Type type);

    virtual bool needsClock() const { return false; }

    void executeProcess();

    // NOT hates only support one input. All other types support unlimited
    // inputs.
    unsigned int addInput();

protected:
    Type type;
    bool computeOutput();
};

#endif