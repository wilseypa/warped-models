#ifndef LOCATION_OBJECT_H
#define LOCATION_OBJECT_H

#include "SimulationObject.h"
#include "LocationState.h"
#include "Person.h"
#include "RandomNumGen.h"
#include "DiseaseModel.h"
#include "DiffusionNetwork.h"
#include "EpidemicEvent.h"

#include <vector>
#include <map>

using namespace std;

/* LocationObject Class */

class LocationObject : public SimulationObject {

public:

    /* Default Constructor */
    LocationObject( string locationName,
                    float transmissibility,
                    unsigned int latentDwellTime,
                    unsigned int incubatingDwellTime,
                    unsigned int infectiousDwellTime,
                    unsigned int asymptDwellTime,
                    float latentInfectivity,
                    float incubatingInfectivity,
                    float infectiousInfectivity,
                    float asymptInfectivity,
                    float probULU,
                    float probULV,
                    float probURV,
                    float probUIV,
                    float probUIU,
                    bool dataCaptureStatus,
                    unsigned int locStateRefreshInterval,
                    unsigned int locDiffusionTrigInterval,
                    vector <Person *> *personVec,
                    unsigned int travelTimeToHub,
                    unsigned int diseaseSeed,
                    unsigned int diffusionSeed );

    /* Destructor */
    ~LocationObject();

    /* Initialize the simulation object */
    void initialize();

    /* Finalize the simulation object before termination */
    void finalize();

    /* Execute the scheduled event */
    void executeProcess();

    /* Allocate a new state */
    State* allocateState();

    /* Deallocate a state */
    void deallocateState(const State *state);

    /* Reclaim an event */
    void reclaimEvent(const Event *event);

    /* Accessor for objectName */
    const string &getName() const {return locationName;}

    void populateTravelMap( map <string, unsigned int> travelMap ) {
        diffusionNetwork->populateTravelCost(travelMap);
    }

private:

    /* Create and send the refresh location state event */
    void refreshLocStateEvent( IntVTime currentTime );

    /* Create and send the diffusion trigger event */
    void triggerDiffusionEvent( IntVTime currentTime );

    /* Migrate person(s) to different location event */
    void migrateLocationEvent(  IntVTime currentTime, 
                                LocationState *locationState  );

    /* Location name */
    string locationName;

    /* Disease Random Number Generator */
    RandomNumGen *diseaseRandGen;

    /* Diffusion Random Number generator */
    RandomNumGen *diffusionRandGen;

    /* Disease model */
    DiseaseModel *diseaseModel;

    /* Diffusion Network */
    DiffusionNetwork *diffusionNetwork;

    /* Initial population */
    vector <Person *> *personVec;

    /* Data capture status */
    bool dataCaptureStatus;

    /* Location state refresh interval */
    unsigned int locStateRefreshInterval;

    /* Location diffusion trigger interval */
    unsigned int locDiffusionTrigInterval;
};

#endif
