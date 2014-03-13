#ifndef WATTS_STROGATZ_MODEL
#define WATTS_STROGATZ_MODEL

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the
// U.S., and the terms of this license.

class WattsStrogatzModel {

public:

    /* Constructor */
    WattsStrogatzModel( unsigned int K, float beta ): 
        K(K), beta(beta) {}

    /* Default destructor */
    ~WattsStrogatzModel() {}

    /* Populate the nodes */
    void populateNodes( vector <string> nodeVec ) {
        this->nodeVec = nodeVec;
    }

    /* Map the nodes using the model's algorithm */
    void mapNodes() {
    }

private:

    /* N nodes */
    vector <string> nodeVec;

    /* K */
    unsigned int K;

    /* Beta */
    float beta;
};

#endif
