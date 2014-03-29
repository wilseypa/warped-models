#include "LogicGate.h"

#include "ComponentState.h"
#include "SignalEvent.h"
#include <IntVTime.h>
#include <stdexcept>
#include <string>


LogicGate::LogicGate(std::string name, unsigned int propagationDelay,
                     unsigned int clockPeriod, Type type)
    : Component(name, propagationDelay, clockPeriod), type(type) {}

unsigned int LogicGate::addInput() {
    if (type == Type::NOT && getNumberOfInputs() >= 1) {
        throw std::runtime_error(std::string("NOT Gates only support one input"));
    }
    return Component::addInput();
}

bool LogicGate::computeOutput() {
    switch (type) {
    case Type::NOT:
        return ~(getComponentState()->inputs[0]);
    case Type::AND:
        for (bool input : getComponentState()->inputs) {
            if (!input) {
                return false;
            }
        }
        return true;
    case Type::OR:
        for (bool input : getComponentState()->inputs) {
            if (input) {
                return true;
            }
        }
        return false;
    case Type::XOR: {
        // There are multiple types of N-input XOR gates.
        // This one uses parity style, which is much more common.
        bool result = false;
        for (bool input : getComponentState()->inputs) {
            result ^= input;
        }
        return result;
    }
    case Type::NAND:
        for (bool input : getComponentState()->inputs) {
            if (!input) {
                return true;
            }
        }
        return false;
    case Type::NOR:
        for (bool input : getComponentState()->inputs) {
            if (input) {
                return false;
            }
        }
        return true;
    }
}

void LogicGate::executeProcess() {
    while (haveMoreEvents() == true) {
        const SignalEvent* event = dynamic_cast<const SignalEvent*>(getEvent());
        if (event == nullptr) {
            auto e = getEvent();
            std::cout << getName() << " received " << e->getDataType() << " at " <<
                      getSimulationTime() << "\n";
        }
        ASSERT(event != nullptr);

        // The output only changes if the input changes
        if (getComponentState()->inputs[event->getInputIndex()] != event->getInputValue()) {
            getComponentState()->inputs[event->getInputIndex()] = event->getInputValue();
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