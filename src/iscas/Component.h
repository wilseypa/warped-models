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

    // Add an input port and return the index of the new port Some input
    // types only support a limited number of inputs. An exception will be
    // raised if this function is called more times than the component
    // supports.
    virtual unsigned int addInput();

    // Connect the output to a specified input port of a component
    void addOutput(Component* component, unsigned int inputIndex);

    // Get the number of inputs currently present
    unsigned int getNumberOfInputs() const;

    // If needsClock() returns true, initialize will schedule a clock event
    void initialize();

    const std::string& getName() const { return name; }
    State* allocateState();

protected:
    ComponentState* getComponentState();
    unsigned int propagationDelay;
    unsigned int clockPeriod;
    std::string name;
    std::vector<std::pair<Component*, unsigned int>> outputs;

    // These are only necessary because objects aren't allowed to initialize
    // their own state.
    unsigned int numberOfInputs;
    std::vector<std::pair<std::string, unsigned int>> outputObjectNames;
};

#endif