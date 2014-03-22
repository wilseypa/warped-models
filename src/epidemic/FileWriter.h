#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include "SimulationObject.h"
#include "SimulationStream.h"
#include "FileState.h"
#include "FileEvent.h"

using namespace std;

/* FileWriter Class */

class FileWriter : public SimulationObject {

public:

    /* Default Constructor */
    FileWriter( string writerName, string fileName ) :
        writerName(writerName), fileName(fileName) {}

    /* Destructor */
    ~FileWriter() {
        deallocateState(getState());
    }

    /* Initialize the simulation object */
    void initialize() {
        outStream = openOutputFile();
    }

    /* Finalize the simulation object before termination */
    void finalize() {}

    /* Execute the scheduled event */
    void executeProcess() {

        FileState *fileState = static_cast<FileState*>(getState());
        IntVTime currentTime = dynamic_cast<const IntVTime&> (getSimulationTime());
        FileEvent *fileEvent = NULL;

        while(haveMoreEvents() == true) {

            fileEvent = (FileEvent *) getEvent();
            if ( fileEvent != NULL ) {
                ostringstream strStream;
                strStream << currentTime.getApproximateIntTime() << endl;
                outStream->insert( strStream );
                outStream->flush();
            }
        }
    }

    /* Allocate a new state */
    State* allocateState() {
        return ( (State *) new FileState() );
    }

    /* Deallocate a state */
    void deallocateState(const State *state) {
        delete state;
    }

    /* Reclaim an event */
    void reclaimEvent(const Event *event) {
        delete event;
    }

    /* Accessor for objectName */
    const string &getName() const {
        return writerName;
    }

private:

    SimulationStream* openOutputFile() {
        return getOFStream(fileName, ios::out);
    }

    /* Writer Name */
    string writerName;

    /* File Name */
    string fileName;

    /* Simulation out stream */
    SimulationStream *outStream;
};

#endif
