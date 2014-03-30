#include "Component.h"
#include "ClockEvent.h"
#include "ComponentState.h"

#include <State.h>
#include <IntVTime.h>
#include <utility>
#include <string>

Component::Component(std::string name, unsigned int propagationDelay, unsigned int clockPeriod)
    : name(name), propagationDelay(propagationDelay), clockPeriod(clockPeriod) {}

void Component::addOutput(Component* component, unsigned int inputIndex) {
    outputObjectNames.push_back(std::make_pair(component->getName(), inputIndex));
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
