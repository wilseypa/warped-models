#include "DFlipFlop.h"

#include "SignalEvent.h"
#include "ComponentState.h"
#include "ClockEvent.h"
#include <IntVTime.h>
#include <string>
#include <stdexcept>

DFlipFlop::DFlipFlop(std::string name, unsigned int propagationDelay, unsigned int clockPeriod)
    : Component(name, propagationDelay, clockPeriod), previousOutput(false) {}

unsigned int DFlipFlop::addInput() {
    if (getNumberOfInputs() >= 1) {
        throw std::runtime_error(std::string("Flip flops only support one input"));
    }
    return Component::addInput();
}


void DFlipFlop::executeProcess() {
    while (haveMoreEvents() == true) {
        auto event = getEvent();

        if (auto signalEvent = dynamic_cast<const SignalEvent*>(event)) {
            getComponentState()->inputs[signalEvent->getInputIndex()] = signalEvent->getInputValue();
            return;
        }

        if (dynamic_cast<const ClockEvent*>(event)) {
            IntVTime currentTime = dynamic_cast<const IntVTime&>(getSimulationTime());
            IntVTime recvTime = currentTime + propagationDelay;
            if (getComponentState()->inputs[0] != previousOutput) {
                previousOutput = getComponentState()->inputs[0];
                // Send the new output signal to all connected components
                for (auto& it : outputs) {
                    it.first->receiveEvent(new SignalEvent(
                                               getSimulationTime(),
                                               recvTime,
                                               this,
                                               it.first,
                                               it.second,
                                               getComponentState()->inputs[0]
                                           ));
                }
            }

            // Schedule another clock tick
            recvTime = currentTime + clockPeriod;
            receiveEvent(new ClockEvent(getSimulationTime(), recvTime, this, this));
            return;
        }
    }
}
