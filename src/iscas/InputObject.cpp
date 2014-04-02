#include "InputObject.h"

#include "SignalEvent.h"
#include "ClockEvent.h"
#include "InputState.h"
#include <IntVTime.h>
#include <string>
#include <stdexcept>

// Seed the generator with a hash of the object's name so that each input is
// unique and consistent between runs.
InputObject::InputObject(std::string name, unsigned int propagationDelay, unsigned int clockPeriod)
    : Component(name, propagationDelay, clockPeriod), distribution(0, 1) {}

State* InputObject::allocateState() {
    return new InputState(name);
}

void InputObject::executeProcess() {
    auto state = static_cast<InputState*>(getState());
    while (haveMoreEvents() == true) {
        auto event = getEvent();

        if (dynamic_cast<const ClockEvent*>(event)) {
            IntVTime currentTime = dynamic_cast<const IntVTime&>(getSimulationTime());
            // Send a random signal to all connected components
            IntVTime recvTime = currentTime + propagationDelay;

            bool value = distribution(state->generator) == 1;
            for (auto& it : outputs) {
                it.first->receiveEvent(new SignalEvent(
                                           getSimulationTime(),
                                           recvTime,
                                           this,
                                           it.first,
                                           it.second,
                                           value
                                       ));
            }
            // Schedule another clock tick
            recvTime = currentTime + clockPeriod;
            receiveEvent(new ClockEvent(getSimulationTime(), recvTime, this, this));
            return;
        }
    }
}
