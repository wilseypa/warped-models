#ifndef WATTS_STROGATZ_MODEL
#define WATTS_STROGATZ_MODEL

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the
// U.S., and the terms of this license.

#define BETA_PRECISION 10000

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
        for( unsigned int uiIndex = 0; uiIndex < nodeVec.size(); uiIndex++ ) {
            delete[] connectionMap[uiIndex];
        }
        delete[] connectionMap;
    }

    /* Populate the nodes */
    void populateNodes( vector <string> nodeVec ) {

        this->nodeVec = nodeVec;
        unsigned int N = nodeVec.size();
        connectionMap = new bool*[N];

        for( unsigned int uiIndex = 0; uiIndex < N; uiIndex++ ) {
            connectionMap[uiIndex] = new bool[N];
            for( unsigned int uiCol = 0; uiCol < N; uiCol++ ) {
                connectionMap[uiIndex][uiCol] = false;
            }
        }
    }

    /* Map the nodes using the model's algorithm */
    void mapNodes() {

        unsigned int N = nodeVec.size();
        unsigned int left  = K/2;
        unsigned int right = K - left;
        unsigned precisionBeta = (unsigned int) (beta*BETA_PRECISION);

        /* Setup the ring lattice with N nodes, each of degree K */
        for( unsigned int uiIndex = 0; uiIndex < N; uiIndex++ ) {

            for( unsigned int uiNodeIndex = 1; uiNodeIndex <= left; uiNodeIndex++ ) {

                unsigned int leftIndex = (uiIndex + N - uiNodeIndex)%N;
                connectionMap[uiIndex][leftIndex] = true;
                connectionMap[leftIndex][uiIndex] = true;
            }
            for( unsigned int uiNodeIndex = 1; uiNodeIndex <= right; uiNodeIndex++ ) {

                unsigned int rightIndex = (uiIndex + uiNodeIndex)%N;
                connectionMap[uiIndex][rightIndex] = true;
                connectionMap[rightIndex][uiIndex] = true;
            }
        }

        /* Rewire each edge with probability beta */
        for( unsigned int uiIndex = 0; uiIndex < N; uiIndex++ ) {

            for( unsigned int uiNodeIndex = 0; uiNodeIndex < N; uiNodeIndex++ ) {

                if( !connectionMap[uiIndex][uiNodeIndex] ) continue;
                unsigned int randomNum = randNumGen->genRandNum( BETA_PRECISION );
                if( randomNum >= precisionBeta ) continue;

                unsigned int uiNewIndex = 0;
                while(1) {
                    uiNewIndex = randNumGen->genRandNum(N);
                    if( (uiNewIndex != uiIndex) && (uiNewIndex != uiNodeIndex) ) break;
                }
                connectionMap[uiIndex][uiNodeIndex] = false;
                connectionMap[uiNodeIndex][uiIndex] = false;
                connectionMap[uiIndex][uiNewIndex]  = false;
                connectionMap[uiNewIndex][uiIndex]  = false;
            }
        }
    }

    /* Send the node links for a particular node */
    vector<string> fetchNodeLinks( string locationName ) {

        unsigned int count = 0;
        for( vector<string>::iterator vecIter = nodeVec.begin(); 
                                vecIter != nodeVec.end(); vecIter++ ) {
            if( *vecIter == locationName ) break;
            else count++;
        }
        if( count == nodeVec.size() ) {
            cerr << "Watts-Strogatz model: Invalid fetch request." << endl;
            abort();
        }

        vector<string> connVec; 
        for( unsigned int uiIndex = 0; uiIndex < nodeVec.size(); uiIndex++ ) {
            if( connectionMap[count][uiIndex] ) {
                connVec.push_back(nodeVec[uiIndex]);
            }
        }
        return connVec;
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

    /* Map of the actual graph */
    bool **connectionMap;
};

#endif
