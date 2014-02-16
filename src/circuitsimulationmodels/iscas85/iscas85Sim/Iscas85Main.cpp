#include "Iscas85Application.h"
#include "WarpedMain.h"

#include "tclap/CmdLine.h"

#include <string>
#include <iostream>

int
main(int argc, char** argv) {
    std::string inputFileName = "";
    std::string testCaseFileName = "";

    std::string configuration = "";
    std::string simulateUntil = "";
    bool debug = false;
    try {
        TCLAP::CmdLine cmd("Iscas85 Simulation");

        TCLAP::ValueArg<string> testCaseFileNameArg("", "simulate", "test case file name",
                true, testCaseFileName, "file", cmd);

        // Arguments for WarpedMain
        TCLAP::ValueArg<std::string> configurationArg("c", "configuration", "WARPED configuration file",
                                                      false, configuration, "file", cmd);
        TCLAP::ValueArg<std::string> simulateUntilArg("u", "simulateUntil", "specify a simulation end time",
                                                      false, simulateUntil, "time", cmd);
        TCLAP::SwitchArg debugArg("d", "debug", "display debug messages", cmd, debug);

        cmd.parse(argc, argv);

        inputFileName = testCaseFileNameArg.getValue();
        testCaseFileName = testCaseFileNameArg.getValue();

        configuration = configurationArg.getValue();
        simulateUntil = simulateUntilArg.getValue();
        debug = debugArg.getValue();
    } catch (TCLAP::ArgException& e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        exit(-1);
    }

    if (inputFileName.empty()) {
        std::cerr << "A configuration file must be specified using --simulate" << std::endl;
        abort();
    }

    // Note that this simulation treats the --simulate argument differently than
    // other simulations.
    string path = "circuitsimulationmodels/iscas85/iscas85Sim/";
    inputFileName = path + inputFileName + "/" + inputFileName + "config";

    WarpedMain wm(new Iscas85Application(inputFileName, testCaseFileName),
                  configuration, simulateUntil, debug);

    return wm.main(argc, argv);
}

