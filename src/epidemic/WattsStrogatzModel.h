#ifndef WATTS_STROGATZ_MODEL
#define WATTS_STROGATZ_MODEL

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the
// U.S., and the terms of this license.

class WattsStrogatzModel {

public:

    /* Populate the nodes */
    void populateNodes( vector <string> nodeVec ) {
        this->nodeVec = nodeVec;
    }

private:
    vector <string> nodeVec;
};

#endif
