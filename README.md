# WAPRED Simulation Models
Simulation models for the WARPED Parallel & Distributed Discrete Simulation Library

# Building

The is built with a C++11 compiler (see [here](http://lektiondestages.blogspot.de/2013/05/installing-and-switching-gccg-versions.html) for instructions about upgrading and switching GCC versions if you have an old version of GCC that does not support the required features.).  

These models require the WARPED library to be built and installed. The library can be found with build instructions [here](https://github.com/wilseypa/pdes). 

To build from the git repository, first clone a local copy.

	git clone https://github.com/wilseypa/warped-models.git ~/warped-models

You can run the Autotools build without any options. These models are not typically installed, so a prefix is optional. However, if the WARPED library is not install at the default location of `/usr`, then the location must be specified with the `--with-warped` configure option. For example, if the WARPED library is installed in `/usr/local`, then the following command would be used to build the models:

	autoreconf -i && ./configure --with-warped=/usr/local && make

# Usage

Each model defines it's own command line interface, but in general they follow the same convention. Each requires at a minimum a runtime configuration file, which must be a JSON file. The path to the configuration file is usually specified with the `-c` or `--configuraion` command line switch. Example configuration files can be found in the `config` directory. 

###Examples

All examples are run from the `src` directory where the executables are built.

Run the Ping Pong model with a sequential simulator.

    ./pingpong -c sequential.json 

Run the RAID model in a parallel configuration. 

    ./raidSim -c parallel.json --simulate raid/LargeRaid

Run the PHOLD model until the global VTime reaches 10000

    ./pholdSim -c parallel.json --simulate phold/LargePHOLD -u 10000

# License
The WARPED Models code in this repository is licensed under the MIT license, unless otherwise specified. The full text of the MIT license can be found in the `LICENSE.txt` file. 

The WARPED models depend on some third party libraries which are redistributed in the `thirdparty/` folder. Each third party library used is licensed under a license that can be found at the top of each file for which the license applies.