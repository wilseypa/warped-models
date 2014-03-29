#include "ISCASApplication.h"

#include "ClockEvent.h"
#include "SignalEvent.h"
#include "InputObject.h"
#include "DFlipFlop.h"
#include "LogicGate.h"
#include <re2/re2.h>
#include <PartitionInfo.h>
#include <RoundRobinPartitioner.h>
#include <DeserializerManager.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <stdexcept>

namespace {
std::string get_file_contents(std::string filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return (contents);
    }
    throw std::runtime_error("Cannot open bench file");
}
} //namespace

ISCASApplication::ISCASApplication(std::string benchFileName, unsigned int propagationDelay,
                                   unsigned int clockPeriod)
    : benchFileName(benchFileName), propagationDelay(propagationDelay),
      clockPeriod(clockPeriod) {}

void
ISCASApplication::registerDeserializers() {
    DeserializerManager::instance()->registerDeserializer(ClockEvent::getClockEventDataType(),
                                                          &ClockEvent::deserialize);
    DeserializerManager::instance()->registerDeserializer(SignalEvent::getSignalEventDataType(),
                                                          &SignalEvent::deserialize);
}

std::vector<SimulationObject*>* ISCASApplication::getSimulationObjects() {
    std::map<std::string, Component*> componentsByName;

    std::string contents = get_file_contents(benchFileName);
    re2::StringPiece input(contents);

    // Create inputs
    std::string group1;
    std::string group2;
    re2::RE2 regex1("INPUT\\(([^)]+)\\)");
    while (RE2::FindAndConsume(&input, regex1, &group1)) {
        componentsByName[group1] = new InputObject(group1, propagationDelay, clockPeriod);
    }

    // Find create unconnected components
    input = re2::StringPiece(contents);
    re2::RE2 regex2("(\\w+) = (\\w+)");
    while (RE2::FindAndConsume(&input, regex2, &group1, &group2)) {
        if (group2 == "DFF") {
            componentsByName[group1] = new DFlipFlop(group1, propagationDelay, clockPeriod);
        } else if (group2 == "NOT") {
            componentsByName[group1] = new LogicGate(group1, propagationDelay, clockPeriod,
                                                     LogicGate::Type::NOT);
        } else if (group2 == "AND") {
            componentsByName[group1] = new LogicGate(group1, propagationDelay, clockPeriod,
                                                     LogicGate::Type::AND);
        } else if (group2 == "OR") {
            componentsByName[group1] = new LogicGate(group1, propagationDelay, clockPeriod,
                                                     LogicGate::Type::OR);
        } else if (group2 == "XOR") {
            componentsByName[group1] = new LogicGate(group1, propagationDelay, clockPeriod,
                                                     LogicGate::Type::XOR);
        } else if (group2 == "NAND") {
            componentsByName[group1] = new LogicGate(group1, propagationDelay, clockPeriod,
                                                     LogicGate::Type::NAND);
        } else if (group2 == "NOR") {
            componentsByName[group1] = new LogicGate(group1, propagationDelay, clockPeriod,
                                                     LogicGate::Type::NOR);
        } else {
            std::cout << "Invalid component type: " << group2 << '\n';
        }
    }

    // Connect all components
    input = re2::StringPiece(contents);
    re2::RE2 regex3("(\\w+) = \\w+\\(([^)]+)\\)");
    re2::RE2 regex4("(\\w+)");
    while (RE2::FindAndConsume(&input, regex3, &group1, &group2)) {
        re2::StringPiece input2(group2);
        std::string arg;
        while (RE2::FindAndConsume(&input2, regex4, &arg)) {
            auto inputIndex = componentsByName[group1]->addInput();
            componentsByName[arg]->addOutput(componentsByName[group1], inputIndex);
        }
    }


    auto objects = new std::vector<SimulationObject*>;
    for (auto it : componentsByName) {
        objects->push_back(it.second);
    }
    return objects;
}