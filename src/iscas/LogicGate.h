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
    State* allocateState();

    // Add an input port and return the index of the new port.
    // NOT gates only support one input. All other types support unlimited
    // inputs. An exception will be raised if this function is called more
    // times than the component supports.
    unsigned int addInput();

private:
    Type type;
    bool computeOutput();
    unsigned int numberOfInputs;
};

#endif