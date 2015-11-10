#include "TrafficApplication.h"
#include "WarpedMain.h"

#include "tclap/CmdLine.h"

#include <string>
#include <iostream>

int main(int argc, char** argv) {

    unsigned int num_intersections_x   = 100;
    unsigned int num_intersections_y   = 100;
    unsigned int num_cars              = 25;
    unsigned int mean_interval         = 400;

    std::string configuration = "";
    std::string simulateUntil = "";
    bool debug = false;

    try {
        TCLAP::CmdLine cmd("Traffic Simulation");
        TCLAP::ValueArg<unsigned int> num_intersections_x_arg("x", "num-intersections-x", 
            "Width of intersection grid", false, num_intersections_x, "unsigned int", cmd);
        TCLAP::ValueArg<unsigned int> num_intersections_y_arg("y", "num-intersections-y", 
            "Height of intersection grid", false, num_intersections_y, "unsigned int", cmd);
        TCLAP::ValueArg<unsigned int> num_cars_arg("n", "number-of-cars", 
            "Number of cars per intersection", false, num_cars, "unsigned int", cmd);
        TCLAP::ValueArg<unsigned int> mean_interval_arg("i", "mean-interval", 
            "Mean interval", false, mean_interval, "unsigned int", cmd);

        // Arguments for WarpedMain
        TCLAP::ValueArg<std::string> configurationArg("c", "configuration", "WARPED configuration file",
                                                      false, configuration, "file", cmd);
        TCLAP::ValueArg<std::string> simulateUntilArg("u", "simulateUntil", "specify a simulation end time",
                                                      false, simulateUntil, "time", cmd);
        TCLAP::SwitchArg debugArg("d", "debug", "display debug messages", cmd, debug);

        cmd.parse(argc, argv);

        num_intersections_x = num_intersections_x_arg.getValue();
        num_intersections_y = num_intersections_y_arg.getValue();
        num_cars            = num_cars_arg.getValue();
        mean_interval       = mean_interval_arg.getValue();

        configuration = configurationArg.getValue();
        simulateUntil = simulateUntilArg.getValue();
        debug = debugArg.getValue();

    } catch (TCLAP::ArgException& e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        exit(-1);
    }

    WarpedMain wm(new TrafficApplication(num_intersections_x, 
                            num_intersections_y, num_cars, mean_interval),
                  configuration, simulateUntil, debug);

    return wm.main(argc, argv);
}
