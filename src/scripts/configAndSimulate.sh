#!/bin/bash

# Allows batch runs of simulations. Saves results to log files

cd ..

function control_c()
# run if user hits control-c
{
  echo -en "\n*** Ouch! Exiting ***\n"
#  scripts/dropbox_uploader.sh upload $logFile
  exit $?
}

#Modifies the config file to the given settings
function set_config {
    workerThreadCount=$1
    scheduleQScheme=$2
    causalityType=$3
    scheduleQCount=$4

    # Copy config file to tmp folder
    cp parallel.json /tmp/$hostname.parallel.json
    configFile="/tmp/$hostname.parallel.json"

    # Set configuration parameters
    sed -i "s/\"WorkerThreadCount\": [0-9]*$/\"WorkerThreadCount\": $workerThreadCount/g" $configFile
    sed -i "s/\"ScheduleQScheme\": \"[a-zA-Z]*$\"/\"ScheduleQScheme\": \"$scheduleQScheme\"/g" $configFile
    sed -i "s/\"CausalityType\": \"[a-zA-Z]*$\"/\"CausalityType\": \"$causalityType\"/g" $configFile
    sed -i "s/\"ScheduleQCount\": [0-9]*$/\"ScheduleQCount\": $scheduleQCount/g" $configFile
}

function run {
    testCycles=$1
    binary=$2
    binaryConfig=$3
    workerThreadCount=$4
    scheduleQScheme=$5
    causalityType=$6
    scheduleQCount=$7
    simulateUntil=$8
    timeoutVal=$9

    set_config $workerThreadCount $scheduleQScheme $causalityType $scheduleQCount

    for ((iteration=1; iteration <= $testCycles; ++iteration));
    do
        echo -e "\nStarting $binary $binaryConfig Simulation: $workerThreadCount threads, $scheduleQCount $causalityType $scheduleQScheme Test number: $iteration/$testCycles\n"
        if [ $simulateUntil == "-" ]
        then
            runCommand="./$binary -c $configFile --simulate $binaryConfig"
        else
            runCommand="./$binary -c $configFile --simulate $binaryConfig -u $simulateUntil"
        fi

        if [ $timeoutVal == "-" ]
        then
            grepMe=`$runCommand | grep "Simulation complete"`
        else
            grepMe=`timeout $timeoutVal bash -c "$runCommand" | grep "Simulation complete"`
        fi
        runTime=`echo $grepMe | sed -e 's/.*complete (\(.*\) secs.*/\1/'`
        rollbacks=`echo $grepMe | sed -e 's/.*Rollbacks: (\(.*\)).*/\1/'`
        echo $runTime
        echo $rollbacks

        # Write to log file
        echo "$binary,$binaryConfig,$iteration,$workerThreadCount,$scheduleQScheme,$causalityType,$scheduleQCount,$simulateUntil,$runTime,$rollbacks" >> $logFile

        sleep 10
    done
}

hostname=`hostname`
date=`date +"%m-%d-%y_%T"`
logFile="scripts/logs/$hostname---$date.csv"

# Write csv header
## Simulation Threads Scheme ScheduleQCount SimulateUntil Runtime Rollbacks
echo "Simulation,SimulationConfig,TestNumber,Threads,Scheme,CausalityType,ScheduleQCount,SimulateUntil,Runtime,Rollbacks" > $logFile

trap control_c SIGINT

. scripts/$1
# run raidSim raid/LargeRAID 4 LadderQueue Strict 2 100000

# Upload output to dropbox
#scripts/dropbox_uploader.sh upload $logFile
