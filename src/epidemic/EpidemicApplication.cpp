#include "EpidemicApplication.h"
#include "LocationObject.h"
#include "EpidemicPartitioner.h"
#include "Person.h"
#include "tinyxml2.h"
#include "PartitionInfo.h"
#include "RoundRobinPartitioner.h"
#include "DeserializerManager.h"
#include "WattsStrogatzModel.h"

#include <vector>
#include <map>
#include <iostream>
#include <fstream>

#define INIT_VTIME 0

using namespace std;
using namespace tinyxml2;


EpidemicApplication::EpidemicApplication(string inputFileName):
    inputFileName(inputFileName) { }

const PartitionInfo* EpidemicApplication::getPartitionInfo(
        unsigned int numberOfProcessorsAvailable,
        const std::vector<SimulationObject*>* simulationObjects) {
    const PartitionInfo* retval = myPartitioner->partition(NULL, numberOfProcessorsAvailable);
    delete myPartitioner;
    return retval;
}

std::vector<SimulationObject*>* EpidemicApplication::getSimulationObjects() {

    myPartitioner = new EpidemicPartitioner();
    int numRegions = 0, numLocations = 0, numPersons = 0;
    unsigned int pid = 0, travelTimeToHub = 0, latentDwellTime = 0, 
            incubatingDwellTime = 0, infectiousDwellTime = 0, 
            asymptDwellTime = 0, locStateRefreshInterval = 0,
            locDiffusionTrigInterval = 0;
    double susceptibility = 0.0;
    float transmissibility = 0.0, probULU = 0.0, probULV = 0.0, 
            probURV = 0.0, probUIV = 0.0, probUIU = 0.0, 
            latentInfectivity = 0.0, incubatingInfectivity = 0.0, 
            infectiousInfectivity = 0.0, asymptInfectivity = 0.0;
    string locationName = "", infectionState = "", vaccinationStatus = "", 
            regionName = "", modelType = "";

    Person *person;
    vector <SimulationObject*> *locObjs;
    vector <Person *> *personVec;

    vector <SimulationObject *> *simulationObjVec = new vector <SimulationObject *>;

    map <string, SimulationObject *> simulationObjMap;
    map <string, unsigned int> travelMap;

    /* open the XML file */
    XMLDocument EpidemicConfig;
    int errorID = EpidemicConfig.LoadFile(inputFileName.c_str());
    if(errorID) {
        cerr << "ERROR : XML config file failed to load. Error ID: " << errorID << endl;
        abort();
    }

    XMLElement *networkModel = NULL, *disease = NULL, *numOfRegions = NULL,
               *region = NULL, *location = NULL, *people = NULL;

    /* Refer to README for more details */
    networkModel = EpidemicConfig.FirstChildElement()->FirstChildElement("network_model");
    modelType.assign( networkModel->GetText() );

    /* Refer to README for more details */
    disease = EpidemicConfig.FirstChildElement()->FirstChildElement("disease");
    disease->FirstChildElement("transmissibility")->QueryFloatText(&transmissibility);
    disease->FirstChildElement("latent_dwell_time")->QueryUnsignedText(&latentDwellTime);
    disease->FirstChildElement("latent_infectivity")->QueryFloatText(&latentInfectivity);
    disease->FirstChildElement("incubating_dwell_time")->QueryUnsignedText(&incubatingDwellTime );
    disease->FirstChildElement("incubating_infectivity")->QueryFloatText(&incubatingInfectivity);
    disease->FirstChildElement("infectious_dwell_time")->QueryUnsignedText(&infectiousDwellTime);
    disease->FirstChildElement("infectious_infectivity")->QueryFloatText(&infectiousInfectivity);
    disease->FirstChildElement("asympt_dwell_time")->QueryUnsignedText(&asymptDwellTime);
    disease->FirstChildElement("asympt_infectivity")->QueryFloatText(&asymptInfectivity);
    disease->FirstChildElement("prob_ul_u")->QueryFloatText(&probULU);
    disease->FirstChildElement("prob_ul_v")->QueryFloatText(&probULV);
    disease->FirstChildElement("prob_ur_v")->QueryFloatText(&probURV);
    disease->FirstChildElement("prob_ui_v")->QueryFloatText(&probUIV);
    disease->FirstChildElement("prob_ui_u")->QueryFloatText(&probUIU);
    disease->FirstChildElement("location_state_refresh_interval")->QueryUnsignedText(&locStateRefreshInterval);

    /* Refer to README for more details */
    numOfRegions=EpidemicConfig.FirstChildElement()->FirstChildElement("number_of_regions");
    numOfRegions->QueryIntText(&numRegions);
    region=numOfRegions;

    /* For each region in the simulation, initialize the locations */
    for( int regIndex = 0; regIndex < numRegions; regIndex++ ) {
        region =region->NextSiblingElement();
        regionName.assign( region->FirstChildElement("region_name")->GetText() );

        numLocations = 0;
        region->FirstChildElement("number_of_locations")->QueryIntText(&numLocations);
        location = region->FirstChildElement("number_of_locations");
        locObjs = new vector<SimulationObject*>;  
        for( int locIndex = 0; locIndex < numLocations; locIndex++ ) {
            personVec = new vector <Person *>;

            location = location->NextSiblingElement();
            locationName.assign( location->FirstChildElement("location_name")->GetText() );          
            location->FirstChildElement("number_of_persons")->QueryIntText(&numPersons);            
            location->FirstChildElement("travel_time_to_central_hub")->QueryUnsignedText(&travelTimeToHub);         
            location->FirstChildElement("diffusion_trigger_interval")->QueryUnsignedText(&locDiffusionTrigInterval);            
            people = location->FirstChildElement("diffusion_trigger_interval");

            locationName += ",";
            locationName += regionName;

            travelMap.insert( pair <string, unsigned int>(locationName, travelTimeToHub) );

            /* Read each person's details */
            for(int perIndex = 0; perIndex < numPersons; perIndex++) {
                people= people->NextSiblingElement();
                people->FirstChildElement("pid")->QueryUnsignedText(&pid);
                people->FirstChildElement("susceptibility")->QueryDoubleText(&susceptibility);
                vaccinationStatus.assign( people->FirstChildElement("is_vaccinated")->GetText() );
                infectionState.assign( people->FirstChildElement("infection_state")->GetText() );

                person = new Person( pid, susceptibility, vaccinationStatus, infectionState, INIT_VTIME, INIT_VTIME );
                personVec->push_back( person );
            }

            LocationObject *locObject = new LocationObject( locationName, transmissibility,
                                                            latentDwellTime, incubatingDwellTime,
                                                            infectiousDwellTime, asymptDwellTime,
                                                            latentInfectivity, incubatingInfectivity,
                                                            infectiousInfectivity, asymptInfectivity,
                                                            probULU, probULV, probURV, probUIV, probUIU,
                                                            locStateRefreshInterval, locDiffusionTrigInterval, 
                                                            personVec, travelTimeToHub);
            locObjs->push_back(locObject);
            simulationObjMap.insert( pair <string, SimulationObject *>(locationName, locObject) );
            simulationObjVec->push_back(locObject);
        }

        /* Add the group of objects to the partition information */
        myPartitioner->addObjectGroup(locObjs);
    }

    /* Send the travel map to each location based on the model type */
    if( modelType == "Random" ) {
        /* Send travel distance of all locations except its own */
        for( map <string, SimulationObject*>::iterator mapIter = simulationObjMap.begin();
                                            mapIter != simulationObjMap.end(); mapIter++ ) {

            LocationObject *locObj = static_cast <LocationObject *> (mapIter->second);
            map <string, unsigned int> tempTravelMap = travelMap;
            tempTravelMap.erase(mapIter->first);
            locObj->populateTravelMap(tempTravelMap);
        }

    } else if( modelType == "WattsStrogatz" ) {
        /* Refer to README for more details */
        unsigned int k = 0;
        float beta = 0.0;
        XMLElement *wattsStrogatz = NULL;
        wattsStrogatz = EpidemicConfig.FirstChildElement()->FirstChildElement("watts_strogatz");
        wattsStrogatz->FirstChildElement("k")->QueryUnsignedText(&k);
        wattsStrogatz->FirstChildElement("beta")->QueryFloatText(&beta);

        WattsStrogatzModel wsModel(k, beta);

        vector <string> nodeVec;
        for( map <string, unsigned int>::iterator mapIter = travelMap.begin();
                                            mapIter != travelMap.end(); mapIter++ ) {

            string location = static_cast <string> (mapIter->first);
            nodeVec.push_back(location);
        }
        wsModel.populateNodes(nodeVec);
        wsModel.mapNodes();

        // to be added soon

    } else {
        cerr << "ERROR : Invalid network model" << endl;
        abort();
    }

    EpidemicConfig.SaveFile(inputFileName.c_str());

    return simulationObjVec;
}

int EpidemicApplication::finalize() {
    return 0;
}

void EpidemicApplication::registerDeserializers() {
    DeserializerManager::instance()->registerDeserializer(  EpidemicEvent::getEpidemicEventDataType(), 
                                                            &EpidemicEvent::deserialize );
}
