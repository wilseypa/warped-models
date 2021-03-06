#include "LocationObject.h"
#include "LocationState.h"
#include "IntVTime.h"

#include <vector>
#include <map>

#define SAMPLING_FREQ 20

using namespace std;

LocationObject::LocationObject( string locationName,
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
                                unsigned int diffusionSeed) : 
        locationName(locationName),
        personVec(personVec),
        dataCaptureStatus(dataCaptureStatus),
        countIntraLocDiseaseUpdate(0),
        locStateRefreshInterval(locStateRefreshInterval),
        locDiffusionTrigInterval(locDiffusionTrigInterval) {

    /* Create and seed the random number class */
    diseaseRandGen   = new RandomNumGen(diseaseSeed);
    diffusionRandGen = new RandomNumGen(diffusionSeed);

    /* Create the disease model */
    diseaseModel = new DiseaseModel(    transmissibility, latentDwellTime, 
                                        incubatingDwellTime, infectiousDwellTime, 
                                        asymptDwellTime, latentInfectivity, 
                                        incubatingInfectivity, infectiousInfectivity, 
                                        asymptInfectivity, probULU, probULV, probURV, 
                                        probUIV, probUIU, diseaseRandGen );

    /* Create the diffusion network */
    diffusionNetwork = new DiffusionNetwork( diffusionRandGen, travelTimeToHub );
}

LocationObject::~LocationObject() {
    deallocateState(getState());

    delete personVec;
    delete diseaseRandGen;
    delete diffusionRandGen;
    delete diseaseModel;
    delete diffusionNetwork;
}

void LocationObject::initialize() {
    LocationState *myState = dynamic_cast<LocationState*>(getState());
    myState->populateLocation(personVec);

    /* Create and send the initial events */
    IntVTime currentTime = static_cast<const IntVTime&> (getSimulationTime());
    refreshLocStateEvent(currentTime);
    triggerDiffusionEvent(currentTime);
}

void LocationObject::finalize() {}

void LocationObject::executeProcess() {

    LocationState *myState = static_cast<LocationState*>(getState());
    IntVTime currentTime = dynamic_cast<const IntVTime&> (getSimulationTime());
    EpidemicEvent *recvEvent = NULL;
    SimulationObject *receiver = NULL;

    while(haveMoreEvents() == true) {

        recvEvent = (EpidemicEvent *) getEvent();
        if ( recvEvent != NULL ) {

            /* Check if the event was sent by the same location */
            if( recvEvent->getSender() == *(this->getObjectID()) ) {

                if( DISEASE == recvEvent->getDiseaseOrDiffusion() ) {
                    diseaseEventAndDataCapture( myState->getPersonMap(), currentTime );

                } else if( DIFFUSION == recvEvent->getDiseaseOrDiffusion() ){
                    migrateLocationEvent(currentTime, myState);
                    triggerDiffusionEvent(currentTime);

                } else {
                    cerr << "Invalid event at " << locationName << endl;
                    abort();
                }
            } else {
                int timeSpentInCurrState = 
                        currentTime.getApproximateIntTime() - 
                                            recvEvent->getTimeSpentInCurrState();
                ASSERT(timeSpentInCurrState >= 0);
                Person *person = new Person(recvEvent->getPID(),
                                            recvEvent->getSusceptibility(),
                                            recvEvent->getVaccinationStatus(),
                                            recvEvent->getInfectionState(),
                                            currentTime.getApproximateIntTime(),
                                            timeSpentInCurrState);
                (myState->getPersonMap())->insert( 
                                std::pair <unsigned int, Person *> (recvEvent->getPID(), person) );
            }
        }
    }
}

State* LocationObject::allocateState() {
    return (new LocationState());
}

void LocationObject::deallocateState( const State *state ) {
    delete state;
}

void LocationObject::reclaimEvent(const Event *event) {
    delete event;
}

void LocationObject::refreshLocStateEvent( IntVTime currentTime ) {

    EpidemicEvent *refreshEvent = new EpidemicEvent(    currentTime,
                                                        currentTime + (int)locStateRefreshInterval,
                                                        this, this, NULL, DISEASE  );
    this->receiveEvent(refreshEvent);
}

void LocationObject::sendCapturedData( IntVTime currentTime,
                                       unsigned int uninfectedNum,
                                       unsigned int latentNum,
                                       unsigned int incubatingNum,
                                       unsigned int infectiousNum,
                                       unsigned int asymptNum,
                                       unsigned int recoveredNum ) {

    SimulationObject *receiver = getObjectHandle("writer");
    FileEvent *fileEvent = new FileEvent( currentTime, currentTime, 
                                          this, receiver,
                                          locationName, countIntraLocDiseaseUpdate, 
                                          uninfectedNum, latentNum, incubatingNum,
                                          infectiousNum, asymptNum, recoveredNum);
    receiver->receiveEvent(fileEvent);
}

void LocationObject::diseaseEventAndDataCapture( map <unsigned int, Person *> *personMap,
                                                 IntVTime currentTime ) {

    unsigned int uninfectedNum = 0, latentNum = 0, incubatingNum = 0,
                    infectiousNum = 0, asymptNum = 0, recoveredNum = 0;

    diseaseModel->diseaseReaction(  personMap,
                                    currentTime.getApproximateIntTime(),
                                    &uninfectedNum,
                                    &latentNum,
                                    &incubatingNum,
                                    &infectiousNum,
                                    &asymptNum,
                                    &recoveredNum );

    refreshLocStateEvent(currentTime);

    /* Send data to the file writer */
    if(dataCaptureStatus) {
        sendCapturedData( currentTime, uninfectedNum,
                          latentNum, incubatingNum,
                          infectiousNum, asymptNum,
                          recoveredNum );
    }

    countIntraLocDiseaseUpdate++;
}

void LocationObject::triggerDiffusionEvent( IntVTime currentTime ) {

    EpidemicEvent *diffusionEvent = new EpidemicEvent(  currentTime,
                                                        currentTime + (int)locDiffusionTrigInterval,
                                                        this, this, NULL, DIFFUSION  );
    this->receiveEvent(diffusionEvent);
}

void LocationObject::migrateLocationEvent(  IntVTime currentTime,
                                            LocationState *locationState  ) {

    /* Randomly decide whether to migrate any person */
    if( !diffusionRandGen->genRandNum(SAMPLING_FREQ) ) {

        /* Decide a random location */
        string selectedLocation = diffusionNetwork->getLocationName();
        if( "" != selectedLocation ) {

            int travelTime = (int) ( diffusionNetwork->travelTimeToChosenLoc(selectedLocation) );
            SimulationObject *receiver = getObjectHandle(selectedLocation);

            /* Decide a random person */
            Person *person = diffusionNetwork->getPerson( locationState->getPersonMap() );

            if(person) {
                EpidemicEvent *migrateLocEvent = new EpidemicEvent( currentTime,
                                                                    currentTime + travelTime,
                                                                    this,
                                                                    receiver,
                                                                    person,
                                                                    DIFFUSION );
                receiver->receiveEvent(migrateLocEvent);
                locationState->deletePersonFromLocation(person);
            }
        }
    }
}


