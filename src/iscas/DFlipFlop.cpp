#include "DFlipFlop.h"

#include "ClockEvent.h"
#include "DFlipFlopState.h"
#include "SignalEvent.h"
#include <IntVTime.h>
#include <string>
#include <stdexcept>

DFlipFlop::DFlipFlop(std::string name, unsigned int propagationDelay, unsigned int clockPeriod)
    : Component(name, propagationDelay, clockPeriod) {}

State* DFlipFlop::allocateState() {
    return new DFlipFlopState();
}

void DFlipFlop::executeProcess() {
    auto state = static_cast<DFlipFlopState*>(getState());
    while (haveMoreEvents() == true) {
        auto event = getEvent();

        if (auto signalEvent = dynamic_cast<const SignalEvent*>(event)) {
            state->input = signalEvent->getInputValue();
            return;
        }

        if (dynamic_cast<const ClockEvent*>(event)) {
            IntVTime currentTime = dynamic_cast<const IntVTime&>(getSimulationTime());
            IntVTime recvTime = currentTime + propagationDelay;
            if (state->input != state->previousOutput) {
                state->previousOutput = state->input;
                // Send the new output signal to all connected components
                for (auto& it : outputs) {
                    it.first->receiveEvent(new SignalEvent(
                                               getSimulationTime(),
                                               recvTime,
                                               this,
                                               it.first,
                                               it.second,
                                               state->input
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
