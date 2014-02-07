#ifndef RANDOM_NUM_GEN_H
#define RANDOM_NUM_GEN_H

#include <stdlib.h>
#include <time.h>

class RandomNumGen {

public:

    /* Default constructor */
    RandomNumGen(unsigned int seedVal): 
        seed(seedVal) {}

    /* Destructor */
    ~RandomNumGen() {}

    /* Generate the random number */
    unsigned int genRandNum( unsigned int upperLimit ) {

        unsigned int randNum = 0;
        if(upperLimit > 0) {
            randNum = (unsigned int) rand_r(&seed)%upperLimit;
        } else {
            randNum = (unsigned int) rand_r(&seed);
        }
        return randNum;
    }

private:
    unsigned int seed;
};

#endif
