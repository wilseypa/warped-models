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
        for( vector< vector<unsigned int> *>::iterator vecIter = nodeConnVec.begin(); 
                                            vecIter != nodeConnVec.end(); vecIter++ ) {
            delete *vecIter;
        }
    }

    /* Populate the nodes */
    void populateNodes( vector <string> nodeVec ) {

        this->nodeVec = nodeVec;
        for( vector<string>::iterator vecIter = nodeVec.begin(); 
                                vecIter != nodeVec.end(); vecIter++ ) {
            vector<unsigned int> *connectionVec = new vector<unsigned int> (K);
            nodeConnVec.push_back (connectionVec);
        }
    }

    /* Map the nodes using the model's algorithm */
    void mapNodes() {
    }

    /* Send the node links for a particular node */
    vector<unsigned int> *fetchNodeLinks( string locationName ) {

        unsigned int count = 0;
        for( vector<string>::iterator vecIter = nodeVec.begin(); 
                                vecIter != nodeVec.end(); vecIter++ ) {
            if( *vecIter == locationName ) break;
            else count++;
        }
        if( count == nodeVec.size() ) return NULL;

        return nodeConnVec[count];
    }

private:

    /* N nodes */
    vector<string> nodeVec;

    /* K */
    unsigned int K;

    /* Beta */
    float beta;

    /* Random number generator */
    RandomNumGen *randNumGen;

    /* Structure of the actual graph */
    vector< vector<unsigned int> *> nodeConnVec;
};

#endif
