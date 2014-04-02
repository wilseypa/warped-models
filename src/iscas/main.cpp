#include "ISCASApplication.h"
#include "WarpedMain.h"

#include "tclap/CmdLine.h"

#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    unsigned int propagationDelay = 1;
    unsigned int clockPeriod = 1000;
    std::string benchFileName = "";

    std::string configuration = "";
    std::string simulateUntil = "";
    bool debug = false;

    try {
        TCLAP::CmdLine cmd("ISCAS Simulation");

        TCLAP::ValueArg<int> propagationDelayArg("", "propagationDelay",
                                                 "the time for a signal to travel between components",
                                                 false, propagationDelay, "int", cmd);
        TCLAP::ValueArg<int> clockPeriodArg("", "clockPeriod",
                                            "the delay between clock ticks",
                                            false, clockPeriod, "int", cmd);
        TCLAP::ValueArg<std::string> benchFileNameArg("s", "simulate", "bench file name",
                                                      true, benchFileName, "file", cmd);

        // Arguments for WarpedMain
        TCLAP::ValueArg<std::string> configurationArg("c", "configuration", "WARPED configuration file",
                                                      false, configuration, "file", cmd);
        TCLAP::ValueArg<std::string> simulateUntilArg("u", "simulateUntil",
                                                      "specify a simulation end time",
                                                      false, simulateUntil, "time", cmd);
        TCLAP::SwitchArg debugArg("d", "debug", "display debug messages", cmd, debug);

        cmd.parse(argc, argv);

        propagationDelay = propagationDelayArg.getValue();
        clockPeriod = clockPeriodArg.getValue();
        benchFileName = benchFileNameArg.getValue();

        configuration = configurationArg.getValue();
        simulateUntil = simulateUntilArg.getValue();
        debug = debugArg.getValue();

    } catch (TCLAP::ArgException& e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        exit(-1);
    }

    WarpedMain wm(new ISCASApplication(benchFileName, propagationDelay, clockPeriod),
                  configuration, simulateUntil, debug);

    return wm.main(argc, argv);
}
