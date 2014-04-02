#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <vector>
#include <utility>

#include <SimulationObject.h>

class State;
class ComponentState;

class Component : public SimulationObject {
public:
    Component(std::string name, unsigned int propagationDelay, unsigned int clockPeriod);

    // Indicates whether of not objects of a class should receive clock tick events.
    virtual bool needsClock() const = 0;

    // Connect the output to a specified input port of a component
    void addOutput(Component* component, unsigned int inputIndex);

    // If needsClock() returns true, initialize will schedule a clock event
    void initialize();

    const std::string& getName() const { return name; }

protected:
    unsigned int propagationDelay;
    unsigned int clockPeriod;
    std::string name;
    std::vector<std::pair<Component*, unsigned int>> outputs;
    std::vector<std::pair<std::string, unsigned int>> outputObjectNames;
};

#endif