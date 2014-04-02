#include "LogicGate.h"

#include "LogicGateState.h"
#include "SignalEvent.h"
#include <IntVTime.h>
#include <stdexcept>
#include <string>


LogicGate::LogicGate(std::string name, unsigned int propagationDelay,
                     unsigned int clockPeriod, Type type)
    : Component(name, propagationDelay, clockPeriod), type(type) {}

State* LogicGate::allocateState() {
    return new LogicGateState(numberOfInputs);
}

unsigned int LogicGate::addInput() {
    if (type == Type::NOT && numberOfInputs >= 1) {
        throw std::runtime_error(std::string("NOT Gates only support one input"));
    }
    return ++numberOfInputs;
}

bool LogicGate::computeOutput() {
    auto state = static_cast<LogicGateState*>(getState());

    switch (type) {
    case Type::NOT:
        return !(state->inputs[0]);
    case Type::AND:
        for (bool input : state->inputs) {
            if (!input) {
                return false;
            }
        }
        return true;
    case Type::OR:
        for (bool input : state->inputs) {
            if (input) {
                return true;
            }
        }
        return false;
    case Type::XOR: {
        // There are multiple types of N-input XOR gates.
        // This one uses parity style, which is much more common.
        bool result = false;
        for (bool input : state->inputs) {
            result ^= input;
        }
        return result;
    }
    case Type::NAND:
        for (bool input : state->inputs) {
            if (!input) {
                return true;
            }
        }
        return false;
    case Type::NOR:
        for (bool input : state->inputs) {
            if (input) {
                return false;
            }
        }
        return true;
    }
}

void LogicGate::executeProcess() {
    auto state = static_cast<LogicGateState*>(getState());
    while (haveMoreEvents() == true) {
        const SignalEvent* event = dynamic_cast<const SignalEvent*>(getEvent());
        ASSERT(event != nullptr);

        // The output only changes if the input changes
        if (state->inputs[event->getInputIndex()] != event->getInputValue()) {
            state->inputs[event->getInputIndex()] = event->getInputValue();
            // Send the new output signal to all connected components
            IntVTime recvTime = dynamic_cast<const IntVTime&>(getSimulationTime()) + propagationDelay;
            for (auto& it : outputs) {
                it.first->receiveEvent(new SignalEvent(
                                           getSimulationTime(),
                                           recvTime,
                                           this,
                                           it.first,
                                           it.second,
                                           computeOutput()
                                       ));
            }
        }
    }
}