#include "Component.h"
#include "ClockEvent.h"
#include "ComponentState.h"

#include <State.h>
#include <IntVTime.h>
#include <utility>
#include <string>

Component::Component(std::string name, unsigned int propagationDelay, unsigned int clockPeriod)
    : name(name), propagationDelay(propagationDelay), clockPeriod(clockPeriod),
      numberOfInputs(0) {}

State* Component::allocateState() {
    return new ComponentState(getNumberOfInputs());
}

ComponentState* Component::getComponentState() {
    return static_cast<ComponentState*>(getState());
}

unsigned int Component::addInput() {
    return ++numberOfInputs;
}

void Component::addOutput(Component* component, unsigned int inputIndex) {
    outputObjectNames.push_back(std::make_pair(component->getName(), inputIndex));
}

unsigned int Component::getNumberOfInputs() const {
    return numberOfInputs;
}

void Component::initialize() {
    // Set up the outputs with actual object pointers
    for (auto& it : outputObjectNames) {
        auto handle = static_cast<Component*>(getObjectHandle(it.first));
        outputs.push_back(std::make_pair(handle, it.second));
    }

    // If necessary, start the clock ticks
    if (needsClock()) {
        IntVTime recvTime = dynamic_cast<const IntVTime&>(getSimulationTime()) + clockPeriod;
        receiveEvent(new ClockEvent(getSimulationTime(), recvTime, this, this));
    }
}
