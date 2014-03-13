#ifndef WATTS_STROGATZ_MODEL
#define WATTS_STROGATZ_MODEL

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the
// U.S., and the terms of this license.

class WattsStrogatzModel {

public:

    /* Constructor */
    WattsStrogatzModel( unsigned int K, float beta, unsigned int seed ): 
            K(K), beta(beta) {

        randNumGen = new RandomNumGen(seed);
    }

    /* Default destructor */
    ~WattsStrogatzModel() {
        delete randNumGen;
    }

    /* Populate the nodes */
    void populateNodes( vector <string> nodeVec ) {
        this->nodeVec = nodeVec;
    }

    /* Map the nodes using the model's algorithm */
    void mapNodes() {
    }

    /* Send the node links for a particular node */
    vector <string> fetchNodeLinks( string ) {
        return nodeVec;
    }

private:

    /* N nodes */
    vector <string> nodeVec;

    /* K */
    unsigned int K;

    /* Beta */
    float beta;

    /* Random number generator */
    RandomNumGen *randNumGen;
};

#endif
